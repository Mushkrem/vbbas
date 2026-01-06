#include "documenttab.h"
#include <QVBoxLayout>
#include <qtoolbar.h>

DocumentTab::DocumentTab(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QToolBar *toolsToolBar = new QToolBar("Tools Toolbar", this);
    toolsToolBar->setObjectName("toolstoolbar");
    toolsToolBar->setIconSize(QSize(16,16));
    toolsToolBar->setFloatable(false);

    QAction *saveAction = new QAction(
        QIcon::fromTheme("document-save"),
        "Save",
        this
        );
    toolsToolBar->addAction(saveAction);

    toolsToolBar->addSeparator();

    QAction *printAction = new QAction(
        QIcon::fromTheme("document-print"),
        "Print",
        this
        );
    toolsToolBar->addAction(printAction);

    layout->addWidget(toolsToolBar);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);
    m_modified = true;

    layout->addWidget(m_view);
}

void DocumentTab::initialize()
{

    emit modifiedChanged(m_modified);
}

void DocumentTab::save() {
    // to do
    m_modified = false;
    emit modifiedChanged(m_modified);
}
