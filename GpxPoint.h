#ifndef GPXPOINT_H
#define GPXPOINT_H

#include <QDateTime>

class GpxPoint
{
public:
    double lat() const;
    void setLat(double lat);

    double lon() const;
    void setLon(double lon);

    double ele() const;
    void setEle(double ele);

    QDateTime timeStamp() const;
    void setTimeStamp(const QDateTime &timeStamp);

private:
    double m_lat = 0.0;
    double m_lon = 0.0;
    double m_ele = 0.0;
    QDateTime m_timeStamp;
};

#endif // GPXPOINT_H
