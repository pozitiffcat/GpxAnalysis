#ifndef SPEEDANALYSISDATA_H
#define SPEEDANALYSISDATA_H

#include "GpxAnalysisData.h"

class SpeedAnalysisData : public GpxAnalysisData
{
public:
    explicit SpeedAnalysisData(GpxTrack *gpxTrack);

    qint64 count() const;
    double valueOnPosition(qint64 position) const;
};

#endif // SPEEDANALYSISDATA_H
