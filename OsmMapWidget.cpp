#include "OsmMapWidget.h"

#include <QPainter>

#include <QMouseEvent>
#include <math.h>

OsmMapWidget::OsmMapWidget(QWidget *parent)
    : QWidget(parent)
{
    connect(&m_osmTileManager, &OsmTileManager::imageTileListReady, [this] () {
        repaint();
    });
}

void OsmMapWidget::setGpxTrack(GpxTrack *gpxTrack)
{
    m_gpxTrack = gpxTrack;
    m_currentDistance = 0.0;
    m_offsetX = 0.0;
    m_offsetY = 0.0;
    m_zoom = 10;
    repaint();
}

void OsmMapWidget::setCurrentDistance(double distance)
{
    m_currentDistance = distance;
    repaint();
}

QSize OsmMapWidget::sizeHint() const
{
    return QSize(256, 256);
}

void OsmMapWidget::mousePressEvent(QMouseEvent *event)
{
    m_x = event->x();
    m_y = event->y();
}

void OsmMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_offsetX -= event->x() - m_x;
    m_offsetY -= event->y() - m_y;

    m_x = event->x();
    m_y = event->y();

    repaint();
}

void OsmMapWidget::wheelEvent(QWheelEvent *event)
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

void OsmMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    paintMap(&painter);
    paintRoute(&painter);
}

void OsmMapWidget::paintMap(QPainter *painter)
{
    const int osmTileSize = 256;

    OsmTile osmStartTile = OsmTile::fromLatLon(50.0, 30.0, m_zoom);

    if (m_gpxTrack && m_gpxTrack->pointsCount() != 0)
        osmStartTile = OsmTile::fromLatLon(m_gpxTrack->point(0).lat(), m_gpxTrack->point(0).lon(), m_zoom);

    QList<OsmTile> osmTileListToRequest;

    for (int j = m_offsetY / osmTileSize - 1; j < size().height() / osmTileSize + m_offsetY / osmTileSize + 1; ++j)
    {
        for (int i = m_offsetX / osmTileSize - 1; i < size().width() / osmTileSize + m_offsetX / osmTileSize + 1; ++i)
        {
            auto currentOsmTile = OsmTile::fromXY(osmStartTile.tileX() + i, osmStartTile.tileY() + j, m_zoom);

            if (m_osmTileManager.hasImageInCache(currentOsmTile))
                painter->drawImage(i * osmTileSize - m_offsetX, j * osmTileSize - m_offsetY, m_osmTileManager.imageFromCache(currentOsmTile));
            else {
                osmTileListToRequest.append(currentOsmTile);
            }
        }
    }

    m_osmTileManager.requestImageTileList(osmTileListToRequest);
}

void OsmMapWidget::paintRoute(QPainter *painter)
{
    const int osmTileSize = 256;

    if (!m_gpxTrack || m_gpxTrack->pointsCount() == 0)
        return;

    OsmTile osmStartTile = OsmTile::fromLatLon(m_gpxTrack->point(0).lat(), m_gpxTrack->point(0).lon(), m_zoom);

    QVector<QPointF> points;
    QPointF currentPoint;

    double distanceCurrent = 0.0;
    QPointF pointCurrent;

    for (qint64 i = 0; i < m_gpxTrack->pointsCount(); ++i)
    {
        const auto &gpxPoint = m_gpxTrack->point(i);
        double distance = 0.0;

        if (i != 0)
        {
            const auto &lastGpxPoint = m_gpxTrack->point(i - 1);
            distance = distanceCurrent + lastGpxPoint.distanceTo(gpxPoint);
        }

        QPointF point = osmStartTile.latLonToPoint(osmTileSize, osmTileSize, gpxPoint.lat(), gpxPoint.lon());
        points.append(QPointF(point.x() - m_offsetX, point.y() - m_offsetY));

        if (m_currentDistance >= distanceCurrent && m_currentDistance < distance)
        {
            QPointF endPoint = QPointF(pointCurrent.x() - m_offsetX, pointCurrent.y() - m_offsetY);
            QPointF startPoint = QPointF(point.x() - m_offsetX, point.y() - m_offsetY);

            double m = distance - distanceCurrent;
            double cm = distance - m_currentDistance;
            QPointF oPoint = endPoint - startPoint;
            currentPoint = oPoint * cm / m + startPoint;
        }

        distanceCurrent = distance;
        pointCurrent = point;
    }

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2.0);

    painter->setPen(pen);
    painter->setBrush(palette().highlight().color().darker());

    painter->drawPolyline(points.data(), points.count());
    painter->drawEllipse(currentPoint, 5, 5);
}
