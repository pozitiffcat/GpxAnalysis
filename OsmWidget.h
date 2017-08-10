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
    void paintEvent(QPaintEvent *event);

private:
    void requestTile(int i, int j, const OsmTile &tile);

private:
    GpxTrack *m_gpxTrack = nullptr;
    OsmTileManager m_osmTileManager;
    QHash<QPair<int, int>, QImage> m_tileImageMap;
};

#endif // OSMWIDGET_H
