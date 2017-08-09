#include "ElevationAnalysisData.h"

ElevationAnalysisData::ElevationAnalysisData(GpxTrack *gpxTrack)
    : GpxAnalysisData(gpxTrack)
{
}

qint64 ElevationAnalysisData::count() const
{
    return m_gpxTrack->pointsCount();
}

double ElevationAnalysisData::valueOnPosition(qint64 position) const
{
    return m_gpxTrack->point(position).ele();
}
