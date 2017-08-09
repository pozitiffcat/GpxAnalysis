#include "GpxAnalysisWidget.h"

#include <QPainter>

GpxAnalysisWidget::GpxAnalysisWidget(QWidget *parent)
    : QWidget(parent)
{
}

void GpxAnalysisWidget::addData(GpxAnalysisData *data)
{
    m_gpxAnalysisDataList.append(data);
    repaint();
}

void GpxAnalysisWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    QColor color = palette().highlight().color();
    for (auto gpxAnalysisData : m_gpxAnalysisDataList)
    {
        paintData(&painter, gpxAnalysisData, color);
        color = color.lighter();
    }
}

void GpxAnalysisWidget::paintData(QPainter *painter, GpxAnalysisData *data, const QColor &color)
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
