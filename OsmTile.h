#ifndef OSMTILE_H
#define OSMTILE_H

#include <QUrl>
#include <QRectF>

class OsmTile
{
public:
    static OsmTile fromXY(int tileX, int tileY, int zoom);
    static OsmTile fromLatLon(double lat, double lon, int zoom);

    QUrl url() const;
    QPointF latLonToPoint(int width, int height, double lat, double lon) const;

    int tileX() const;
    int tileY() const;

private:
    OsmTile(int tileX, int tileY, int zoom);

    QRectF boundingBox() const;

    double tileToLon(int x) const;
    double tileToLat(int y) const;

    static double radToDeg(double r);
    static double degToRad(double d);

private:
    const double m_tileX;
    const double m_tileY;
    const int m_zoom;
};

#endif // OSMTILE_H
