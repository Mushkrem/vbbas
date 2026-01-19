#include "algorithmview.h"

#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QApplication>
#include <qmath.h>

AlgorithmView::AlgorithmView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void AlgorithmView::setZoomRange(qreal minZoom, qreal maxZoom)
{
    m_minZoom = minZoom;
    m_maxZoom = maxZoom;
}

void AlgorithmView::wheelEvent(QWheelEvent *event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if (modifiers & Qt::ControlModifier) {
        // Ctrl + Scroll: Zoom
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    }
    else if (modifiers & Qt::ShiftModifier) {
        // Shift + Scroll: Horizontal scroll
        QScrollBar *hBar = horizontalScrollBar();
        int delta = -event->angleDelta().y();
        hBar->setValue(hBar->value() + delta);
        event->accept();
    }
    else {
        // No modifier: Normal vertical scroll
        QGraphicsView::wheelEvent(event);
    }
}

void AlgorithmView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            zoomIn();
            event->accept();
            return;
        case Qt::Key_Minus:
            zoomOut();
            event->accept();
            return;
        case Qt::Key_0:
            resetZoom();
            event->accept();
            return;
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void AlgorithmView::zoomIn()
{
    zoom(m_zoomStep, mapToScene(viewport()->rect().center()));
}

void AlgorithmView::zoomOut()
{
    zoom(1.0 / m_zoomStep, mapToScene(viewport()->rect().center()));
}

void AlgorithmView::resetZoom()
{
    resetTransform();
    m_currentZoom = 1.0;
}

void AlgorithmView::fitInView()
{
    if (!scene()) return;

    QRectF bounds = scene()->itemsBoundingRect();
    if (bounds.isEmpty()) return;

    QGraphicsView::fitInView(bounds, Qt::KeepAspectRatio);

    m_currentZoom = transform().m11();
}

void AlgorithmView::zoom(qreal factor, QPointF centerPoint)
{
    qreal newZoom = m_currentZoom * factor;

    if (newZoom < m_minZoom) {
        factor = m_minZoom / m_currentZoom;
        newZoom = m_minZoom;
    } else if (newZoom > m_maxZoom) {
        factor = m_maxZoom / m_currentZoom;
        newZoom = m_maxZoom;
    }

    scale(factor, factor);
    m_currentZoom = newZoom;

    qDebug() << "Zoom:" << QString::number(m_currentZoom * 100, 'f', 0) << "%";
}
