#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "stylingutils.h"

#include <QStyleHints>
#include <QToolBar>
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

    QTabWidget *central = new QTabWidget;
    central->setObjectName("viewertab");

    central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *tab1 = new QWidget;
    QWidget *tab2 = new QWidget;
    central->setTabsClosable(true);
    central->setMovable(true);

    central->addTab(tab1, "Home");
    central->addTab(tab2, "testfile.vbb");

    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->addWidget(central);

    QList<QAction *> actions = ui->menubar->actions();
    for (QAction *action : actions) {
        QMenu *menu = action->menu();

        Styling::applyDropShadowEffect(menu);
    }
}

void MainWindow::connectActions() {
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::onOpenFile);
}

void MainWindow::onOpenFile() {

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
