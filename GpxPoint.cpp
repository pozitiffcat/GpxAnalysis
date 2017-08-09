#include "GpxPoint.h"

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
