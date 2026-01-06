#include "mainwindow.h"
#include "stylingutils.h"
#include "./ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleHints>
#include <QDockWidget>
#include <QMouseEvent>
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

    resizeDocks({ui->tasksWidget, ui->navigatorWidget, ui->variablesWidget}, {280, 280, 200}, Qt::Horizontal);
    resizeDocks({ui->outputWidget}, {160}, Qt::Vertical);

    setCentralWidget(ui->centralwidget);
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    central = new QTabWidget(this);
    central->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    central->setObjectName("viewertab");
    central->setTabsClosable(true);
    central->setDocumentMode(true);
    central->setMovable(true);

    docksManager = new DocksManager(findChildren<QDockWidget*>(), this);
    documentsManager = new DocumentsManager(central, this);
    actionsManager = new ActionsManager(this);

    setupUI();

    QTabBar *bar = central->tabBar();
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    bar->setElideMode(Qt::ElideRight);
    bar->setUsesScrollButtons(false);
    bar->installEventFilter(this);
    bar->setExpanding(false);
    // bar->setContextMenuPolicy(Qt::CustomContextMenu);

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

    connectActions();
}

void MainWindow::connectActions() {
    actionsManager->file->setDocumentInfo(documentsManager);

    // action → document
    connect(actionsManager->file, &FileActions::newFileRequested,
            documentsManager, &DocumentsManager::createNewDocument);
    connect(actionsManager->file, &FileActions::openFileRequested,
            documentsManager, &DocumentsManager::openDocument);
    connect(actionsManager->file, &FileActions::saveFileRequested,
            documentsManager, &DocumentsManager::saveCurrentDocument);
    connect(actionsManager->file, &FileActions::closeFileRequested,
            documentsManager, static_cast<void (DocumentsManager::*)()>(&DocumentsManager::closeDocument));

    // document → action states
    connect(documentsManager, &DocumentsManager::documentCreated,
            actionsManager->file, &FileActions::updateActionStates);
    connect(documentsManager, &DocumentsManager::documentClosed,
            actionsManager->file, &FileActions::updateActionStates);
    connect(documentsManager, &DocumentsManager::documentOpened,
            actionsManager->file, &FileActions::updateActionStates);
    connect(documentsManager, &DocumentsManager::documentModificationChanged,
            actionsManager->file, &FileActions::updateActionStates);

    // central
    connect(central, &QTabWidget::tabCloseRequested,
            documentsManager, static_cast<void (DocumentsManager::*)(int)>(&DocumentsManager::closeDocument));
    connect(central, &QTabWidget::tabBarClicked,
            actionsManager->file, &FileActions::updateActionStates);
}

void MainWindow::setupUI() {
    actionsManager->setupMenus(ui->menuFile, ui->menuEdit);

    for (QToolBar *toolbar : actionsManager->createToolBars(this)) {
        addToolBar(Qt::TopToolBarArea, toolbar);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == central->tabBar() && event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::MiddleButton) {
            int tabIndex = central->tabBar()->tabAt(mouseEvent->pos());
            if (tabIndex != -1) {
                documentsManager->closeDocument(tabIndex);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
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
