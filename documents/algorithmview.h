#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include <QGraphicsView>

class AlgorithmView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AlgorithmView(QGraphicsScene *scene, QWidget *parent = nullptr);

    void setZoomRange(qreal minZoom, qreal maxZoom);
    qreal currentZoom() const { return m_currentZoom; }

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void fitInView();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void zoom(qreal factor, QPointF centerPoint);

    qreal m_currentZoom = 1.0;
    qreal m_minZoom = 0.1;
    qreal m_maxZoom = 5.0;
    qreal m_zoomStep = 1.15;
};

#endif // ALGORITHMVIEW_H
