#ifndef ANALYSISDATA_H
#define ANALYSISDATA_H

#include <qglobal.h>

#include "GpxTrack.h"

class AnalysisData
{
public:
    explicit AnalysisData(GpxTrack *gpxTrack);
    virtual ~AnalysisData() = default;

    double distance() const;
    double distanceToPosition(qint64 position) const;

    virtual qint64 count() const = 0;
    virtual double valueOnPosition(qint64 position) const = 0;

protected:
    GpxTrack *m_gpxTrack;
    QVector<double> m_distances;
};

#endif // ANALYSISDATA_H
