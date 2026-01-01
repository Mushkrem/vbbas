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

signals:
    void modifiedChanged(bool modified);

private:
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

};

#endif // DOCUMENTTAB_H
