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

void MainWindow::addToolBars() {
    fileToolBar = new QToolBar("File Toolbar", this);
    fileToolBar->setObjectName("filetoolbar");
    fileToolBar->setIconSize(QSize(16,16));
    fileToolBar->setFloatable(false);

    fileToolBar->addAction(fileActions->newFileAction);
    fileToolBar->addAction(fileActions->openFileAction);
    fileToolBar->addAction(fileActions->saveFileAction);

    addToolBar(Qt::TopToolBarArea, fileToolBar);
}

void MainWindow::addActions() {
    ui->menuFile->addAction(fileActions->newFileAction);
    ui->menuFile->addAction(fileActions->openFileAction);
    ui->menuFile->addAction(fileActions->closeFileAction);

    ui->menuFile->addSection(NULL); // separator
    ui->menuFile->addAction(fileActions->saveFileAction);
    ui->menuFile->addAction(fileActions->saveAsFileAction);
    ui->menuFile->addAction(fileActions->saveAllFilesAction);

    ui->menuFile->addSection(NULL); // separator
    ui->menuFile->addAction(fileActions->printFileAction);

    ui->menuFile->addSection(NULL); // separator
    ui->menuFile->addAction(fileActions->recentFilesAction);

    ui->menuFile->addSection(NULL); // separator
    ui->menuFile->addAction(fileActions->exitAction);
}

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
    fileActions = new FileActions(this);

    addToolBars();
    addActions();

    central = new QTabWidget(this);
    central->setObjectName("viewertab");
    central->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    central->setTabsClosable(true);
    central->setMovable(true);

    QTabBar *bar = central->tabBar();
    bar->setExpanding(false);
    bar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
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

    documentsManager = new DocumentsManager(central, this);

    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->addWidget(central, 1);

    QList<QAction *> actions = ui->menubar->actions();
    for (QAction *action : actions) {
        QMenu *menu = action->menu();

        Styling::applyDropShadowEffect(menu);
    }

    connect(fileActions, &FileActions::saveFileRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)()>(&DocumentsManager::saveCurrentDocument));

    connect(fileActions, &FileActions::newFileRequested,
            documentsManager,
            &DocumentsManager::createNewDocument);

    connect(fileActions, &FileActions::openFileRequested,
            documentsManager,
            &DocumentsManager::openDocument);

    connect(fileActions, &FileActions::closeFileRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)()>(&DocumentsManager::closeDocument));

    connect(central, &QTabWidget::tabCloseRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)(int)>(&DocumentsManager::closeDocument));

    QList<QDockWidget*> docks = findChildren<QDockWidget*>();
    docksManager = new DocksManager(docks, this);
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
