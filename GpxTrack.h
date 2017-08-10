#ifndef GPXTRACK_H
#define GPXTRACK_H

#include "GpxPoint.h"

#include <QString>
#include <QVector>

class GpxTrack
{
public:
    explicit GpxTrack(const QString &fileName);

    bool hasError() const;
    QString errorString() const;

    const GpxPoint &point(qint64 position) const;
    qint64 pointsCount() const;

    double elevationGain() const;
    double distance() const;

private:
    QString m_error;
    QVector<GpxPoint> m_gpxPoints;
};

#endif // GPXTRACK_H
