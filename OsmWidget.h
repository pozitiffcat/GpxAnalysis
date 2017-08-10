#ifndef OSMWIDGET_H
#define OSMWIDGET_H

#include <QHash>
#include <QWidget>

#include "OsmTile.h"
#include "GpxTrack.h"
#include "OsmTileManager.h"

class OsmWidget : public QWidget
{ 
public:
    explicit OsmWidget(QWidget *parent = nullptr);

    void setGpxTrack(GpxTrack *gpxTrack);

    QSize sizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void requestTile(int i, int j, const OsmTile &tile);

private:
    GpxTrack *m_gpxTrack = nullptr;
    OsmTileManager m_osmTileManager;
    QHash<QPair<int, int>, QImage> m_tileImageMap;

    double m_x = 0.0;
    double m_y = 0.0;
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
};

#endif // OSMWIDGET_H
