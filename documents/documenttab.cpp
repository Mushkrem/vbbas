#include "documenttab.h"

#include "../MimeTypes.h"

#include "../objects/objectfactory.h"
#include "../objects/connectionitem.h"

#include <QFileInfo>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QApplication>
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
    m_toolsToolBar->setIconSize(QSize(16, 16));
    m_toolsToolBar->setFloatable(false);

    m_editToolBar = new QToolBar("Edit Toolbar", this);
    m_editToolBar->setObjectName("edittoolbar");
    m_editToolBar->setIconSize(QSize(16, 16));
    m_editToolBar->setFloatable(false);

    m_objectToolBar = new QToolBar("Object Toolbar", this);
    m_objectToolBar->setObjectName("objecttoolbar");
    m_objectToolBar->setIconSize(QSize(16, 16));
    m_objectToolBar->setFloatable(false);

    toolbarsLayout->addWidget(m_toolsToolBar);
    toolbarsLayout->addWidget(m_editToolBar);
    toolbarsLayout->addWidget(m_objectToolBar);
    toolbarsLayout->addStretch();

    layout->addLayout(toolbarsLayout);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-2000, -2000, 4000, 4000);
    connect(m_scene, &QGraphicsScene::selectionChanged,
            this, &DocumentTab::onSceneSelectionChanged);

    m_view = new AlgorithmView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->viewport()->installEventFilter(this);
    m_view->installEventFilter(this);

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
    m_objectToolBar->addSeparator();
    for (QAction *action : actions) {
        if(action->toolTip() == "Statement block")
            m_objectToolBar->addSeparator();
        m_objectToolBar->addAction(action);
    }
}

void DocumentTab::initialize()
{
    m_modified = m_filePath.isNull();
    emit modifiedChanged(m_modified);
}



void DocumentTab::save() {
    qDebug() << "DocumentTab Save";
    m_modified = false;
    m_title = m_newTitle;
    emit modifiedChanged(m_modified);
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
    if (m_placingBlock) {
        m_scene->removeItem(m_previewBlock);
        delete m_previewBlock;
        m_previewBlock = nullptr;
    }

    m_previewBlock = ObjectFactory::createBlock(type);
    if (!m_previewBlock) {
        qDebug() << "Failed to create block of type" << static_cast<int>(type);
        return;
    }

    m_previewBlock->setOpacity(0.33);
    m_scene->addItem(m_previewBlock);

    m_placingBlock = true;
    m_blockTypeToPlace = type;
    m_view->setCursor(Qt::CrossCursor);
    m_view->setFocus();
}

void DocumentTab::onSceneSelectionChanged() {
    auto items = m_scene->selectedItems();
    m_selected = items.count();
    emit selectionChanged(m_selected);
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

void DocumentTab::cancelPlacement() {
    if (m_previewBlock) {
        m_scene->removeItem(m_previewBlock);
        delete m_previewBlock;
        m_previewBlock = nullptr;
    }

    m_placingBlock = false;
    m_view->setCursor(Qt::ArrowCursor);
}

void DocumentTab::placeBlockAt(const QPoint &pos) {
    QPointF scenePos = m_view->mapToScene(pos);

    if (auto *block = ObjectFactory::createBlock(m_blockTypeToPlace))
        addBlock(block, scenePos);

    cancelPlacement();
}

void DocumentTab::copySelected() {
    auto items = m_scene->selectedItems();
    if (items.isEmpty())
        return;

    QJsonArray clipboard_array;

    for (auto *item : std::as_const(items)) {
        auto *object = qgraphicsitem_cast<ObjectBase*>(item);
        if (!object)
            continue;

        clipboard_array.append(object->toJson());
    }

    QJsonDocument document(clipboard_array);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    QMimeData *mime = new QMimeData();
    mime->setData(MIME_VIBAS, data);

    QApplication:: // ekkekekekek
    // usun m_clipboard, uzywaj systemowgo - dodalem mime type. musze siku
    emit clipboardChanged(m_clipboard);
}

bool DocumentTab::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_view && m_placingBlock) {
        if (event->type() != QEvent::KeyPress)
            return false;

        auto *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            cancelPlacement();
            return true;
        }

        return false;
    }

    if (watched == m_view->viewport() && m_placingBlock) {
        switch (event->type()) {
        case QEvent::MouseMove: {
            auto *mouse = static_cast<QMouseEvent*>(event);
            QPointF pos = m_view->mapToScene(mouse->pos());
            if (m_previewBlock)
                m_previewBlock->setPos(pos);
            return false;
        }

        case QEvent::MouseButtonPress: {
            auto *mouse = static_cast<QMouseEvent*>(event);

            if (mouse->button() == Qt::LeftButton) {
                placeBlockAt(mouse->pos());
                return true;
            }

            if (mouse->button() == Qt::RightButton) {
                cancelPlacement();
                return true;
            }

            return false;
        }

        default:
            return false;
        }
    }

    return QWidget::eventFilter(watched, event);
}
