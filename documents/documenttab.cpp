#include "documenttab.h"
#include <QVBoxLayout>

DocumentTab::DocumentTab(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    layout->addWidget(m_view);
}
