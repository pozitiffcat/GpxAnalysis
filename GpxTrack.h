#ifndef GPXTRACK_H
#define GPXTRACK_H

#include "GpxPoint.h"

#include <QString>
#include <QVector>

class GpxTrack
{
public:
    bool parse(const QString &fileName);

private:
    QString m_error;
    QVector<GpxPoint> m_gpxPoints;
};

#endif // GPXTRACK_H
