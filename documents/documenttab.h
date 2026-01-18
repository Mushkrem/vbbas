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
    QString filePath() const { return m_filePath; }

    void save();
    void setTitle(const QString &title);
    void setFilePath(const QString &path);
    void initialize();

    void setFileActions(QList<QAction*> actions);
    void setEditActions(QList<QAction*> actions);
    void setObjectActions(QList<QAction*> actions);

    bool isModified() const { return m_modified; };

signals:
    void modifiedChanged(bool modified);

private:
    QString m_newTitle = "untitled.vib";
    QString m_title = "untitled.vib";
    QString m_filePath;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    QToolBar *m_toolsToolBar;
    QToolBar *m_editToolBar;

    bool m_modified;
};

#endif // DOCUMENTTAB_H
