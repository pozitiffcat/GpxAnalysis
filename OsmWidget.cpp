#include "OsmWidget.h"

#include <QPainter>

#include <QMouseEvent>
#include <math.h>

OsmWidget::OsmWidget(QWidget *parent)
    : QWidget(parent)
{
    connect(&m_osmTileManager, &OsmTileManager::imageTileListReady, [this] () {
        repaint();
    });
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

void OsmWidget::mousePressEvent(QMouseEvent *event)
{
    m_x = event->x();
    m_y = event->y();
}

void OsmWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_offsetX -= event->x() - m_x;
    m_offsetY -= event->y() - m_y;

    m_x = event->x();
    m_y = event->y();

    repaint();
}

void OsmWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        m_zoom += 1;
        m_offsetX *= 2.0;
        m_offsetY *= 2.0;
    }
    else if (event->delta() < 0)
    {
        m_zoom -= 1;
        m_offsetX /= 2.0;
        m_offsetY /= 2.0;
    }

    repaint();
}

void OsmWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_gpxTrack->pointsCount() == 0)
        return;

    QPainter painter(this);

    QVector<QPointF> points;

    OsmTile osmStartTile = OsmTile::fromLatLon(m_gpxTrack->point(0).lat(), m_gpxTrack->point(0).lon(), m_zoom);

    for (qint64 i = 0; i < m_gpxTrack->pointsCount(); ++i)
    {
        const auto &gpxPoint = m_gpxTrack->point(i);
        QPointF point = osmStartTile.latLonToPoint(256, 256, gpxPoint.lat(), gpxPoint.lon());
        points.append(QPointF(point.x() - m_offsetX, point.y() - m_offsetY));
    }

    QPen pen(Qt::black);
    pen.setWidthF(2.0);
    painter.setPen(pen);

    QList<OsmTile> osmTileListToRequest;

    for (int j = m_offsetY / 256 - 1; j < size().height() / 256 + m_offsetY / 256 + 1; ++j)
    {
        for (int i = m_offsetX / 256 - 1; i < size().width() / 256 + m_offsetX / 256 + 1; ++i)
        {
            auto currentOsmTile = OsmTile::fromXY(osmStartTile.tileX() + i, osmStartTile.tileY() + j, m_zoom);

            if (m_osmTileManager.hasImageInCache(currentOsmTile))
                painter.drawImage(i * 256 - m_offsetX, j * 256 - m_offsetY, m_osmTileManager.imageFromCache(currentOsmTile));
            else {
                osmTileListToRequest.append(currentOsmTile);
            }
        }
    }

    painter.drawPolyline(points.data(), points.count());

    m_osmTileManager.requestImageTileList(osmTileListToRequest);
}
