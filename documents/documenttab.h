#ifndef DOCUMENTTAB_H
#define DOCUMENTTAB_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class DocumentTab : public QWidget {
    Q_OBJECT
public:
    explicit DocumentTab(QWidget *parent = nullptr);

    QGraphicsScene *scene() const { return m_scene; }
    QGraphicsView *view() const { return m_view; }
    QString title() const { return m_title; }

    void save();

    void setTitle(const QString &title) { m_title = title; }
    void initialize();

signals:
    void modifiedChanged(bool modified);

private:
    QString m_title = "Untitled";
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
};

#endif // DOCUMENTTAB_H
