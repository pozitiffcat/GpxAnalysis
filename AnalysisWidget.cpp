#include "AnalysisWidget.h"

#include <QMouseEvent>
#include <QPainter>

AnalysisWidget::AnalysisWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
}

void AnalysisWidget::addData(AnalysisData *data)
{
    m_gpxAnalysisDataList.append(data);
    repaint();
}

void AnalysisWidget::clearDataSet()
{
    m_gpxAnalysisDataList.clear();
    repaint();
}

double AnalysisWidget::currentDistance() const
{
    return m_currentDistance;
}

void AnalysisWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_gpxAnalysisDataList.isEmpty())
        return;

    double distance = m_gpxAnalysisDataList.first()->distance();
    m_currentDistance = event->x() * distance / size().width();

    repaint();

    emit currentDistanceChanged();
}

void AnalysisWidget::paintEvent(QPaintEvent *event)
{
    if (m_gpxAnalysisDataList.isEmpty())
        return;

    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    QColor color = palette().highlight().color();
    for (auto gpxAnalysisData : m_gpxAnalysisDataList)
    {
        paintData(&painter, gpxAnalysisData, color);
        color = color.lighter();
    }

    double distance = m_gpxAnalysisDataList.first()->distance();
    double currentDistanceX = m_currentDistance * size().width() / distance;

    QPen pen(palette().highlight().color());
    pen.setWidth(1.0);
    painter.setPen(pen);

    painter.drawLine(QPointF(currentDistanceX, 0.0), QPointF(currentDistanceX, size().height()));
}

void AnalysisWidget::paintData(QPainter *painter, AnalysisData *data, const QColor &color)
{
    const double distance = data->distance();
    double min = 99999.9;
    double max = -99999.9;

    QVector<double> values;

    for (qint64 i = 0; i < data->count(); ++i)
    {
        const double value = data->valueOnPosition(i);
        values.append(value);
        min = qMin(min, value);
        max = qMax(max, value);
    }

    QVector<QPointF> points;

    for (qint64 i = 0; i < data->count(); ++i)
    {
        const double value = values.at(i);
        const double distanceTo = data->distanceToPosition(i);
        points.append(QPointF(distanceTo * size().width() / distance, size().height() - (value - min) * size().height() / (max - min)));
    }

    painter->setPen(QPen(color));
    painter->drawPolyline(points.data(), points.count());
}