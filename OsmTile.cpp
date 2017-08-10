#include "OsmTile.h"

#include <math.h>

OsmTile::OsmTile(double lat, double lon, int zoom)
    : m_lat(lat),
      m_lon(lon),
      m_zoom(zoom)
{
}

QUrl OsmTile::url() const
{
    int x = tileX();
    int y = tileY();
    return QUrl::fromUserInput(QString("http://a.tile.openstreetmap.org/%3/%1/%2.png").arg(x).arg(y).arg(m_zoom));
}

QPointF OsmTile::latLonToPoint(int width, int height, double lat, double lon) const
{
    const double x = ((lon + 180.0) * (width / 360.0)) * pow(2.0, m_zoom);
    const double y = ((height / 2.0) - (width * (log(tan((M_PI / 4.0) + (degToRad(lat) / 2.0)))) / (2.0 * M_PI))) * pow(2.0, m_zoom);

    const QRectF bbox = boundingBox();
    const double xOffset = ((bbox.left() + 180.0) * (width / 360.0)) * pow(2.0, m_zoom);
    const double yOffset = ((height / 2.0) - (width * (log(tan((M_PI / 4.0) + (degToRad(bbox.top()) / 2.0)))) / (2.0 * M_PI))) * pow(2.0, m_zoom);

    return QPointF(x - xOffset, y - yOffset);
}

QRectF OsmTile::boundingBox() const
{
    QRectF rect;
    rect.setTop(tileToLat(tileY()));
    rect.setBottom(tileToLat(tileY() + 1));
    rect.setLeft(tileToLon(tileX()));
    rect.setRight(tileToLon(tileX()) + 1);
    return rect;
}

int OsmTile::tileX() const
{
    const double n = pow(2.0, m_zoom);
    return (m_lon + 180.0) / 360.0 * n;
}

int OsmTile::tileY() const
{
    const double latRad = degToRad(m_lat);
    const double n = pow(2.0, m_zoom);
    return (1.0 - log(tan(latRad) + (1.0 / cos(latRad))) / M_PI) / 2.0 * n;
}

double OsmTile::tileToLon(int x) const
{
    return x / pow(2.0, m_zoom) * 360.0 - 180.0;
}

double OsmTile::tileToLat(int y) const
{
    const double n = M_PI - (2.0 * M_PI * y) / pow(2.0, m_zoom);
    return radToDeg(atan(sinh(n)));
}

double OsmTile::radToDeg(double r) const
{
    return r * (180.0 / M_PI);
}

double OsmTile::degToRad(double d) const
{
    return d / (180.0 / M_PI);
}
