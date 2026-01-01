#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "stylingutils.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleHints>
#include <QToolBar>
#include <QTabBar>
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
    setCentralWidget(ui->centralwidget);
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    fileActions = new FileActions(this);

    addToolBars();
    addActions();

    central = new QTabWidget(this);
    central->setObjectName("viewertab");
    central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    central->tabBar()->setExpanding(false);
    central->tabBar()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    central->setTabsClosable(true);
    central->setMovable(true);

    documentsManager = new DocumentsManager(central, this);

    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->addWidget(central);

    QList<QAction *> actions = ui->menubar->actions();
    for (QAction *action : actions) {
        QMenu *menu = action->menu();

        Styling::applyDropShadowEffect(menu);
    }

    connect(fileActions, &FileActions::newFileRequested,
            documentsManager, &DocumentsManager::createNewDocument);

    connect(fileActions, &FileActions::openFileRequested,
            documentsManager, &DocumentsManager::openDocument);

    connect(fileActions, &FileActions::closeFileRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)()>(&DocumentsManager::closeDocument));

    connect(central, &QTabWidget::tabCloseRequested,
            documentsManager,
            static_cast<void (DocumentsManager::*)(int)>(&DocumentsManager::closeDocument));
}

void MainWindow::createNewTab() {
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    auto *scene = new QGraphicsScene;
    auto *view = new QGraphicsView(scene);

    layout->addWidget(view);

    int index = central->addTab(tab, "Untitled");
    central->setCurrentIndex(index);
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
