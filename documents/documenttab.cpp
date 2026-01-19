#include "documenttab.h"

#include "../objects/objectfactory.h"
#include "../objects/connectionitem.h"

#include <QFileInfo>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGraphicsSceneMouseEvent>

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
    m_scene->setSceneRect(-2000, -2000, 4000, 4000);

    m_view = new AlgorithmView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->viewport()->installEventFilter(this);

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

void DocumentTab::onObjectRequested(ObjectActions::ObjectType type)
{
    m_previewBlock = ObjectFactory::createBlock(type);
    if (!m_previewBlock) {
        qDebug() << "Failed to create block of type" << static_cast<int>(type);
        return;
    }

    m_previewBlock->setOpacity(0.5);
    m_scene->addItem(m_previewBlock);

    m_placingBlock = true;
    m_blockTypeToPlace = type;
    m_view->setCursor(Qt::CrossCursor);
}

void DocumentTab::addBlock(ObjectBase *block, const QPointF &pos) {
    if (!block) return;

    block->setPos(pos);
    m_scene->addItem(block);
    m_blocks.append(block);

    m_modified = true;
    emit modifiedChanged(m_modified);
}

void DocumentTab::removeBlock(ObjectBase *block)
{
    if (!block) return;

    m_blocks.removeOne(block);
    m_scene->removeItem(block);
    delete block;

    m_modified = true;
    emit modifiedChanged(true);
}

void DocumentTab::addConnection(ConnectionItem *connection)
{
    if (!connection) return;

    m_scene->addItem(connection);
    m_connections.append(connection);

    m_modified = true;
    emit modifiedChanged(true);
}

void DocumentTab::removeConnection(ConnectionItem *connection)
{
    if (!connection) return;

    m_connections.removeOne(connection);
    m_scene->removeItem(connection);
    delete connection;

    m_modified = true;
    emit modifiedChanged(true);
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

bool DocumentTab::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_view->viewport() && m_placingBlock) {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPointF scenePos = m_view->mapToScene(mouseEvent->pos());

            if (m_previewBlock) {
                m_previewBlock->setPos(scenePos);
            }

            return false;
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton) {
                // Place
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());

                if (m_previewBlock) {
                    m_scene->removeItem(m_previewBlock);
                    delete m_previewBlock;
                    m_previewBlock = nullptr;
                }

                ObjectBase *block = ObjectFactory::createBlock(m_blockTypeToPlace);
                if (block) {
                    addBlock(block, scenePos);
                }

                m_placingBlock = false;
                m_view->setCursor(Qt::ArrowCursor);

                return true;
            }
            else if (mouseEvent->button() == Qt::RightButton) {
                // Cancel
                if (m_previewBlock) {
                    m_scene->removeItem(m_previewBlock);
                    delete m_previewBlock;
                    m_previewBlock = nullptr;
                }

                m_placingBlock = false;
                m_view->setCursor(Qt::ArrowCursor);

                return true;
            }
        }
        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape) {
                // Cancel Esc but doesn't work :(
                if (m_previewBlock) {
                    m_scene->removeItem(m_previewBlock);
                    delete m_previewBlock;
                    m_previewBlock = nullptr;
                }

                m_placingBlock = false;
                m_view->setCursor(Qt::ArrowCursor);

                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}
