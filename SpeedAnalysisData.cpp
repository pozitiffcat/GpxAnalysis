#include "SpeedAnalysisData.h"

SpeedAnalysisData::SpeedAnalysisData(GpxTrack *gpxTrack)
    : GpxAnalysisData(gpxTrack)
{
}

qint64 SpeedAnalysisData::count() const
{
    return m_gpxTrack->pointsCount();
}

double SpeedAnalysisData::valueOnPosition(qint64 position) const
{
    if (position == 0)
        return 0.0;

    return m_gpxTrack->point(position - 1).speedKmhTo(m_gpxTrack->point(position));
}
