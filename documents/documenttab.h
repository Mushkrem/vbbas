#ifndef DOCUMENTTAB_H
#define DOCUMENTTAB_H

#include "algorithmview.h"
#include "../objects/objectbase.h"

#include <QGraphicsScene>
#include <QToolBar>
#include <QWidget>
#include <QJsonArray>

class DocumentTab : public QWidget {
    Q_OBJECT
public:
    explicit DocumentTab(QWidget *parent = nullptr);

    QGraphicsScene *scene() const { return m_scene; }
    AlgorithmView *view() const { return m_view; }
    QString title() const { return m_title; }
    QString newTitle() const { return m_newTitle; }
    QString filePath() const { return m_filePath; }

    void save();
    void setTitle(const QString &title);
    void setFilePath(const QString &path);
    void initialize();

    void setFileActions(QList<QAction*> actions);
    void setEditActions(QList<QAction*> actions);
    void setObjectActions(QList<QAction*> actions);

    bool isModified() const { return m_modified; };
    bool isAnythingSelected() const { return m_selected; };

    QList<ObjectBase*> blocks() const { return m_blocks; }
    QList<ConnectionItem*> connections() const { return m_connections; }

    void addBlock(ObjectBase *block, const QPointF &pos);
    void removeBlock(ObjectBase *block);

    void addConnection(ConnectionItem *connection);
    void removeConnection(ConnectionItem *connection);

public slots:
    void onObjectRequested(ObjectActions::ObjectType type);
    void onSceneSelectionChanged();

signals:
    void clipboardChanged(QJsonArray contents);
    void selectionChanged(int count);
    void modifiedChanged(bool modified);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QString m_newTitle = "untitled.vib";
    QString m_title = "untitled.vib";
    QString m_filePath;
    QGraphicsScene *m_scene;
    AlgorithmView *m_view;

    QToolBar *m_toolsToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_objectToolBar;

    void cancelPlacement();
    void placeBlockAt(const QPoint &pos);

    void copySelected();

    QList<ObjectBase*> m_blocks;
    QList<ConnectionItem*> m_connections;
    bool m_placingBlock = false;
    ObjectActions::ObjectType m_blockTypeToPlace;
    ObjectBase *m_previewBlock = nullptr;

    QJsonArray m_clipboard;

    bool m_modified;
    int m_selected;
};

#endif // DOCUMENTTAB_H
