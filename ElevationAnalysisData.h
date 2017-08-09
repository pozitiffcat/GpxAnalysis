#ifndef ELEVATIONANALYSISDATA_H
#define ELEVATIONANALYSISDATA_H

#include "GpxAnalysisData.h"

class ElevationAnalysisData : public GpxAnalysisData
{
public:
    explicit ElevationAnalysisData(GpxTrack *gpxTrack);

    qint64 count() const;
    double valueOnPosition(qint64 position) const;
};

#endif // ELEVATIONANALYSISDATA_H
