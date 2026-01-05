#include "mainwindow.h"
#include "stylingutils.h"
#include "./ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleHints>
#include <QDockWidget>
#include <QToolBar>
#include <QTabBar>
#include <QStyle>
#include <QEvent>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resizeDocks({ui->outputWidget}, {160}, Qt::Vertical);
    resizeDocks({ui->tasksWidget, ui->navigatorWidget, ui->variablesWidget}, {280, 280, 200}, Qt::Horizontal);

    setCentralWidget(ui->centralwidget);
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    central = new QTabWidget(this);
    central->setObjectName("viewertab");
    central->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    central->setTabsClosable(true);
    central->setMovable(true);
    central->setDocumentMode(true);

    actionsManager = new ActionsManager(this);
    documentsManager = new DocumentsManager(central, this);
    docksManager = new DocksManager(findChildren<QDockWidget*>(), this);

    setupUI();

    QTabBar *bar = central->tabBar();
    bar->setExpanding(false);
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    bar->setUsesScrollButtons(false);
    bar->setElideMode(Qt::ElideRight);

    auto updateSingleProperty = [this]() {
        QTabBar *bar = central->tabBar();
        bool single = (central->count() == 1);
        bar->setProperty("single", single);

        bar->style()->unpolish(bar);
        bar->style()->polish(bar);
        bar->update();
    };

    connect(central, &QTabWidget::currentChanged,
            this, [updateSingleProperty](int){ updateSingleProperty(); });

    connect(central, &QTabWidget::tabCloseRequested,
            this, [updateSingleProperty](int){ updateSingleProperty(); });

    updateSingleProperty();

    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->addWidget(central, 1);

    QList<QAction *> actions = ui->menubar->actions();
    for (QAction *action : actions) {
        QMenu *menu = action->menu();

        Styling::applyDropShadowEffect(menu);
    }

    connect(actionsManager->file, &FileActions::saveFileRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)()>(&DocumentsManager::saveCurrentDocument));

    connect(actionsManager->file, &FileActions::newFileRequested,
            documentsManager,
            &DocumentsManager::createNewDocument);

    connect(actionsManager->file, &FileActions::openFileRequested,
            documentsManager,
            &DocumentsManager::openDocument);

    connect(actionsManager->file, &FileActions::closeFileRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)()>(&DocumentsManager::closeDocument));

    connect(central, &QTabWidget::tabCloseRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)(int)>(&DocumentsManager::closeDocument));
}

void MainWindow::setupUI() {
    actionsManager->setupMenus(ui->menuFile, ui->menuEdit);

    for (QToolBar *toolbar : actionsManager->createToolBars(this)) {
        qDebug() << toolbar;
        addToolBar(Qt::TopToolBarArea, toolbar);
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ThemeChange) {
        auto scheme = QGuiApplication::styleHints()->colorScheme();
        qApp->setPalette(Styling::createCustomPalette(scheme));
        Styling::applyStyling(this);
    }

    QMainWindow::changeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
