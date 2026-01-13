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
    QString newTitle() const { return m_newTitle; }

    void save();
    void setTitle(const QString &title);
    void initialize();

    void setFileActions(QList<QAction*> actions);
    void setEditActions(QList<QAction*> actions);
    void setObjectActions(QList<QAction*> actions);

    bool isModified() const { return m_modified; };

signals:
    void modifiedChanged(bool modified);

private:
    QString m_newTitle = "Untitled";
    QString m_title = "Untitled";
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    QToolBar *m_toolsToolBar;
    QToolBar *m_editToolBar;

    bool m_modified;
};

#endif // DOCUMENTTAB_H
