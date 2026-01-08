#ifndef DOCUMENTTAB_H
#define DOCUMENTTAB_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>
#include <QWidget>

class DocumentTab : public QWidget {
    Q_OBJECT
public:
    explicit DocumentTab(QWidget *parent = nullptr);

    QGraphicsScene *scene() const { return m_scene; }
    QGraphicsView *view() const { return m_view; }
    QString title() const { return m_title; }

    void save();
    bool isModified() const { return m_modified; };

    void setTitle(const QString &title) { m_title = title; }
    void initialize();

    void setFileActions(QList<QAction*> actions);
    void setEditActions(QList<QAction*> actions);

signals:
    void modifiedChanged(bool modified);

private:
    QString m_title = "Untitled";
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    QToolBar *m_toolsToolBar;
    QToolBar *m_editToolBar;

    bool m_modified;
};

#endif // DOCUMENTTAB_H
