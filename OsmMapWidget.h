#ifndef OSMMAPWIDGET_H
#define OSMMAPWIDGET_H

#include <QHash>
#include <QWidget>

#include "OsmTile.h"
#include "GpxTrack.h"
#include "OsmTileManager.h"

class OsmMapWidget : public QWidget
{ 
public:
    explicit OsmMapWidget(QWidget *parent = nullptr);

    void setGpxTrack(GpxTrack *gpxTrack);
    void setCurrentDistance(double distance);

    QSize sizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintMap(QPainter *painter);
    void paintRoute(QPainter *painter);

private:
    GpxTrack *m_gpxTrack = nullptr;
    OsmTileManager m_osmTileManager;

    int m_zoom = 3;
    double m_currentDistance = 0.0;

    double m_x = 0.0;
    double m_y = 0.0;
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
};

#endif // OSMMAPWIDGET_H
