#include "GpxPoint.h"

#include <math.h>

double GpxPoint::lat() const
{
    return m_lat;
}

void GpxPoint::setLat(double lat)
{
    m_lat = lat;
}

double GpxPoint::lon() const
{
    return m_lon;
}

void GpxPoint::setLon(double lon)
{
    m_lon = lon;
}

double GpxPoint::ele() const
{
    return m_ele;
}

void GpxPoint::setEle(double ele)
{
    m_ele = ele;
}

QDateTime GpxPoint::timeStamp() const
{
    return m_timeStamp;
}

void GpxPoint::setTimeStamp(const QDateTime &timeStamp)
{
    m_timeStamp = timeStamp;
}

double GpxPoint::elevationTo(const GpxPoint &gpxPoint) const
{
    return gpxPoint.ele() - this->ele();
}

double GpxPoint::distanceTo(const GpxPoint &gpxPoint) const
{
    double angle1 = angleInDegrees(this->lat());
    double angle2 = angleInDegrees(gpxPoint.lat());

    double pointRadius1 = pointRadius(this->ele(), angle1);
    double pointRadius2 = pointRadius(gpxPoint.ele(), angle2);

    double earthPoint1X = pointRadius1 * cos(degToRad(angle1));
    double earthPoint1Y = pointRadius1 * sin(degToRad(angle1));

    double earthPoint2X = pointRadius2 * cos(degToRad(angle2));
    double earthPoint2Y = pointRadius2 * sin(degToRad(angle2));

    double x = sqrt(pow((earthPoint1X - earthPoint2X), 2.0) + pow((earthPoint1Y - earthPoint2Y), 2.0));
    double y = M_PI *  ((earthPoint1X + earthPoint2X) / 360.0) * (this->lon() - gpxPoint.lon());

    return sqrt(pow(x, 2.0) + pow(y, 2.0));
}

double GpxPoint::secondsTo(const GpxPoint &gpxPoint) const
{
    return this->timeStamp().secsTo(gpxPoint.timeStamp());
}

double GpxPoint::speedKmhTo(const GpxPoint &gpxPoint) const
{
    const auto distance = this->distanceTo(gpxPoint);
    const auto secs = this->secondsTo(gpxPoint);

    const double speedMps = secs != 0 ? distance / secs : 0.0;
    return speedMps != speedMps ? 0.0 : (speedMps * 3600.0) / 1000.0;
}

double GpxPoint::angleInDegrees(double latInDegrees) const
{
    return atan(((pow(6356752.3142, 2.0) / pow(6378137.0, 2.0)) * tan(degToRad(latInDegrees)))) * 180 / M_PI;
}

double GpxPoint::pointRadius(double ele, double angleInDegrees) const
{
    return (1.0 / sqrt((pow(cos(degToRad(angleInDegrees)), 2.0) / pow(6378137.0, 2.0)) + (pow(sin(degToRad(angleInDegrees)), 2.0) / pow(6356752.3142, 2.0)))) + ele;
}

double GpxPoint::degToRad(double deg) const
{
    return deg / (180.0 / M_PI);
}
