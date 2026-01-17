#include "documenttab.h"

#include <QVBoxLayout>
#include <QFileInfo>

DocumentTab::DocumentTab(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QHBoxLayout *toolbarsLayout = new QHBoxLayout();
    toolbarsLayout->setObjectName("document_toolbar");
    toolbarsLayout->setContentsMargins(0,0,0,0);
    toolbarsLayout->setSpacing(0);

    m_toolsToolBar = new QToolBar("Tools Toolbar", this);
    m_toolsToolBar->setObjectName("toolstoolbar");
    m_toolsToolBar->setIconSize(QSize(16,16));
    m_toolsToolBar->setFloatable(false);

    m_editToolBar = new QToolBar("Edit Toolbar", this);
    m_editToolBar->setObjectName("edittoolbar");
    m_editToolBar->setIconSize(QSize(16,16));
    m_editToolBar->setFloatable(false);

    toolbarsLayout->addWidget(m_toolsToolBar);
    toolbarsLayout->addWidget(m_editToolBar);
    toolbarsLayout->addStretch();

    layout->addLayout(toolbarsLayout);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);
    // m_modified = true;

    layout->addWidget(m_view);
}

void DocumentTab::setFileActions(QList<QAction*> actions) {
    for (QAction *action : actions)
        m_toolsToolBar->addAction(action);
}

void DocumentTab::setEditActions(QList<QAction*> actions) {
    m_editToolBar->addSeparator();
    for (QAction *action : actions) {
        if(action->toolTip() == "Undo")
            m_editToolBar->addSeparator();
        m_editToolBar->addAction(action);
    }
}

void DocumentTab::setObjectActions(QList<QAction*> actions) {
    m_editToolBar->addSeparator();
    for (QAction *action : actions) {
        if(action->toolTip() == "Statement block")
            m_editToolBar->addSeparator();
        m_editToolBar->addAction(action);
    }
}


void DocumentTab::setFilePath(const QString &filePath) {
    m_filePath = filePath;

    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        setTitle(fileInfo.fileName());
    }
}

void DocumentTab::setTitle(const QString &title) {
    m_newTitle = title;
    m_modified = true;
    emit modifiedChanged(m_modified);
}

void DocumentTab::initialize()
{
    m_modified = m_filePath.isNull();
    emit modifiedChanged(m_modified);
}

void DocumentTab::save() {
    qDebug() << "DocumentTab Save";
    // to do
    m_modified = false;
    m_title = m_newTitle;
    emit modifiedChanged(m_modified);
}
