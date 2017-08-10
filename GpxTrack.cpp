#include "GpxTrack.h"

#include <QFile>
#include <QStack>
#include <QXmlStreamReader>

GpxTrack::GpxTrack(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_error = QObject::tr("Can't open file");
        return;
    }

    QXmlStreamReader xml(&file);
    QStack<QString> tagStack;
    GpxPoint gpxPoint;

    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();

        if (xml.tokenType() == QXmlStreamReader::StartElement)
            tagStack.append(xml.name().toString().toLower());

        if (tagStack.isEmpty())
            continue;

        if (xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if (tagStack.top() == "trkpt")
            {
                for (auto attribute : xml.attributes())
                {
                    if (attribute.name() == "lat")
                        gpxPoint.setLat(attribute.value().toDouble());

                    if (attribute.name() == "lon")
                        gpxPoint.setLon(attribute.value().toDouble());
                }
            }
        }

        if (xml.tokenType() == QXmlStreamReader::Characters)
        {
            if (tagStack.top() == "ele")
                gpxPoint.setEle(xml.text().toDouble());

            if (tagStack.top() == "time")
                gpxPoint.setTimeStamp(QDateTime::fromString(xml.text().toString(), "yyyy-MM-ddThh:mm:ssZ"));
        }

        if (xml.tokenType() == QXmlStreamReader::EndElement)
        {
            if (tagStack.top() == "trkpt")
            {
                m_gpxPoints.append(gpxPoint);
                gpxPoint = GpxPoint();
            }
        }

        if (xml.tokenType() == QXmlStreamReader::EndElement)
            tagStack.pop();
    }
}

bool GpxTrack::hasError() const
{
    return !m_error.isEmpty();
}

QString GpxTrack::errorString() const
{
    return m_error;
}

const GpxPoint &GpxTrack::point(qint64 position) const
{
    return m_gpxPoints.at(position);
}

qint64 GpxTrack::pointsCount() const
{
    return m_gpxPoints.count();
}

double GpxTrack::elevationGain() const
{
    double acc = 0.0;

    for (qint64 i = 0; i < pointsCount() - 1; ++i)
    {
        const auto elevationTo = point(i).elevationTo(point(i + 1));
        if (elevationTo > 0.0)
            acc += elevationTo;
    }

    return acc;
}

double GpxTrack::distance() const
{
    double acc = 0.0;

    for (qint64 i = 0; i < pointsCount() - 1; ++i)
    {
        const auto distanceTo = point(i).distanceTo(point(i + 1));
        acc += distanceTo;
    }

    return acc;
}
