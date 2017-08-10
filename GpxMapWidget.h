#ifndef GPXMAPWIDGET_H
#define GPXMAPWIDGET_H

#include <QWidget>

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
    double radToDeg(double r) const;
    double degToRad(double d) const;

    void requestTile();

private:
    GpxTrack *m_gpxTrack = nullptr;
    QImage m_tile;
};

#endif // GPXMAPWIDGET_H
