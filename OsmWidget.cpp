#include "OsmWidget.h"

#include <QPainter>

#include <math.h>

OsmWidget::OsmWidget(QWidget *parent)
    : QWidget(parent)
{
}

void OsmWidget::setGpxTrack(GpxTrack *gpxTrack)
{
    m_gpxTrack = gpxTrack;
    repaint();
}

QSize OsmWidget::sizeHint() const
{
    return QSize(256, 256);
}

void OsmWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_gpxTrack->pointsCount() == 0)
        return;

    QPainter painter(this);

    QVector<QPointF> points;

    OsmTile osmStartTile = OsmTile::fromLatLon(m_gpxTrack->point(0).lat(), m_gpxTrack->point(0).lon(), 10);

    for (qint64 i = 0; i < m_gpxTrack->pointsCount(); ++i)
    {
        const auto &gpxPoint = m_gpxTrack->point(i);
        points.append(osmStartTile.latLonToPoint(256, 256, gpxPoint.lat(), gpxPoint.lon()));
    }

    QPen pen(Qt::black);
    pen.setWidthF(2.0);
    painter.setPen(pen);

    for (int j = 0; j < size().height() / 256; ++j)
    {
        for (int i = 0; i < size().width() / 256; ++i)
        {
            auto currentOsmTile = OsmTile::fromXY(osmStartTile.tileX() + i, osmStartTile.tileY() + j, 10);

            if (m_osmTileManager.hasImageInCache(currentOsmTile))
                m_tileImageMap.insert(qMakePair(i, j), m_osmTileManager.imageFromCache(currentOsmTile));

            if (!m_tileImageMap.contains(qMakePair(i, j)))
                requestTile(i, j, currentOsmTile);

            painter.drawImage(i * 256, j * 256, m_tileImageMap.value(qMakePair(i, j)));
        }
    }

    painter.drawPolyline(points.data(), points.count());
}

void OsmWidget::requestTile(int i,int j, const OsmTile &osmTile)
{
    m_osmTileManager.requestTile(osmTile);
    connect(&m_osmTileManager, &OsmTileManager::imageReady, [this, i, j, osmTile] () {
        m_tileImageMap.insert(qMakePair(i, j), m_osmTileManager.imageFromCache(osmTile));
        repaint();
    });
}
