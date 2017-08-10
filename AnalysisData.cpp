#include "AnalysisData.h"

AnalysisData::AnalysisData(GpxTrack *gpxTrack)
    : m_gpxTrack(gpxTrack)
{
    double acc = 0.0;
    m_distances.append(acc);

    for (qint64 i = 1; i < m_gpxTrack->pointsCount(); ++i)
    {
        acc += m_gpxTrack->point(i - 1).distanceTo(m_gpxTrack->point(i));
        m_distances.append(acc);
    }
}

double AnalysisData::distance() const
{
    return m_distances.last();
}

double AnalysisData::distanceToPosition(qint64 position) const
{
    return m_distances.at(position);
}
