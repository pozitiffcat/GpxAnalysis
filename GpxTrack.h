#ifndef GPXTRACK_H
#define GPXTRACK_H

#include "GpxPoint.h"

#include <QString>
#include <QVector>

class GpxTrack
{
public:
    bool parse(const QString &fileName);
    QString errorString() const;

    const GpxPoint &point(qint64 position) const;
    qint64 pointsCount() const;

    double elevationGain() const;

private:
    QString m_error;
    QVector<GpxPoint> m_gpxPoints;
};

#endif // GPXTRACK_H
