#ifndef GPXMAPWIDGET_H
#define GPXMAPWIDGET_H

#include <QWidget>

#include "OsmTile.h"
#include "GpxTrack.h"

class GpxMapWidget : public QWidget
{ 
public:
    explicit GpxMapWidget(QWidget *parent = nullptr);

    void setGpxTrack(GpxTrack *gpxTrack);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void requestTile(const OsmTile &tile);

private:
    GpxTrack *m_gpxTrack = nullptr;
    QImage m_tileImage;
};

#endif // GPXMAPWIDGET_H
