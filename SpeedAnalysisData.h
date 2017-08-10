#ifndef SPEEDANALYSISDATA_H
#define SPEEDANALYSISDATA_H

#include "AnalysisData.h"

class SpeedAnalysisData : public AnalysisData
{
public:
    explicit SpeedAnalysisData(GpxTrack *gpxTrack);

    qint64 count() const;
    double valueOnPosition(qint64 position) const;
};

#endif // SPEEDANALYSISDATA_H
