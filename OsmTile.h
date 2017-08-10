#ifndef OSMTILE_H
#define OSMTILE_H

#include <QUrl>
#include <QRectF>

class OsmTile
{
public:
    OsmTile(double lat, double lon, int zoom);

    QUrl url() const;
    QPointF latLonToPoint(int width, int height, double lat, double lon) const;

private:
    QRectF boundingBox() const;

    int tileX() const;
    int tileY() const;

    double tileToLon(int x) const;
    double tileToLat(int y) const;

    double radToDeg(double r) const;
    double degToRad(double d) const;

private:
    const double m_lat;
    const double m_lon;
    const int m_zoom;
};

#endif // OSMTILE_H
