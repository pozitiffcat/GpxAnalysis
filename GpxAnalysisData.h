#ifndef GPXANALYSISDATA_H
#define GPXANALYSISDATA_H

#include <qglobal.h>

#include "GpxTrack.h"

class GpxAnalysisData
{
public:
    explicit GpxAnalysisData(GpxTrack *gpxTrack);
    virtual ~GpxAnalysisData() = default;

    double distance() const;
    double distanceToPosition(qint64 position) const;

    virtual qint64 count() const = 0;
    virtual double valueOnPosition(qint64 position) const = 0;

protected:
    GpxTrack *m_gpxTrack;
    QVector<double> m_distances;
};

#endif // GPXANALYSISDATA_H
