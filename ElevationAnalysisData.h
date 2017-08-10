#ifndef ELEVATIONANALYSISDATA_H
#define ELEVATIONANALYSISDATA_H

#include "AnalysisData.h"

class ElevationAnalysisData : public AnalysisData
{
public:
    explicit ElevationAnalysisData(GpxTrack *gpxTrack);

    qint64 count() const;
    double valueOnPosition(qint64 position) const;
};

#endif // ELEVATIONANALYSISDATA_H
