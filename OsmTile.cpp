#include "OsmTile.h"

#include <math.h>

OsmTile OsmTile::fromXY(int tileX, int tileY, int zoom)
{
    return OsmTile(tileX, tileY, zoom);
}

OsmTile OsmTile::fromLatLon(double lat, double lon, int zoom)
{
    const double n = pow(2.0, zoom);
    const double latRad = degToRad(lat);
    const double tileX = (lon + 180.0) / 360.0 * n;
    const double tileY = (1.0 - log(tan(latRad) + (1.0 / cos(latRad))) / M_PI) / 2.0 * n;
    return OsmTile(tileX, tileY, zoom);
}

QUrl OsmTile::url() const
{
    int x = tileX();
    int y = tileY();
    return QUrl::fromUserInput(QString("http://a.tile.thunderforest.com/cycle/%3/%1/%2.png?apikey=159a438ecb5f4a809a33feb3d2435b2d").arg(x).arg(y).arg(m_zoom));
    //return QUrl::fromUserInput(QString("http://a.tile.openstreetmap.org/%3/%1/%2.png").arg(x).arg(y).arg(m_zoom));
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

int OsmTile::tileX() const
{
    return m_tileX;
}

int OsmTile::tileY() const
{
    return m_tileY;
}

bool OsmTile::operator ==(const OsmTile &tile) const
{
    return url() == tile.url();
}

OsmTile::OsmTile(int tileX, int tileY, int zoom)
    : m_tileX(tileX),
      m_tileY(tileY),
      m_zoom(zoom)
{
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

double OsmTile::tileToLon(int x) const
{
    return x / pow(2.0, m_zoom) * 360.0 - 180.0;
}

double OsmTile::tileToLat(int y) const
{
    const double n = M_PI - (2.0 * M_PI * y) / pow(2.0, m_zoom);
    return radToDeg(atan(sinh(n)));
}

double OsmTile::radToDeg(double r)
{
    return r * (180.0 / M_PI);
}

double OsmTile::degToRad(double d)
{
    return d / (180.0 / M_PI);
}
