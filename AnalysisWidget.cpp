#include "AnalysisWidget.h"

#include <QPainter>
#include <QMouseEvent>

AnalysisWidget::AnalysisWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
}

void AnalysisWidget::setAnalysisData(AnalysisData *data)
{
    m_analysisData = data;
    repaint();
}

void AnalysisWidget::setCurrentDistance(double distance)
{
    m_currentDistance = distance;
    repaint();
}

double AnalysisWidget::currentDistance() const
{
    return m_currentDistance;
}

void AnalysisWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_analysisData)
        return;

    double distance = m_analysisData->distance();
    m_currentDistance = event->x() * distance / size().width();

    repaint();

    emit currentDistanceChangedByMouse(m_currentDistance);
}

void AnalysisWidget::paintEvent(QPaintEvent *event)
{
    if (!m_analysisData)
        return;

    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    paintData(&painter);
    paintCurrent(&painter);
}

void AnalysisWidget::paintData(QPainter *painter)
{
    const double distance = m_analysisData->distance();
    double min = 99999.9;
    double max = -99999.9;

    QVector<double> values;

    for (qint64 i = 0; i < m_analysisData->count(); ++i)
    {
        const double value = m_analysisData->valueOnPosition(i);
        values.append(value);
        min = qMin(min, value);
        max = qMax(max, value);
    }

    QVector<QPointF> points;

    for (qint64 i = 0; i < m_analysisData->count(); ++i)
    {
        const double value = values.at(i);
        const double distanceTo = m_analysisData->distanceToPosition(i);
        points.append(QPointF(distanceTo * size().width() / distance, size().height() - (value - min) * size().height() / (max - min)));
    }

    QPen pen(Qt::darkCyan);
    pen.setWidth(1.0);
    painter->setPen(pen);

    painter->drawPolyline(points.data(), points.count());
}

void AnalysisWidget::paintCurrent(QPainter *painter)
{
    double distance = m_analysisData->distance();
    double currentDistanceX = m_currentDistance * size().width() / distance;

    QPen pen(Qt::darkMagenta);
    pen.setWidth(1.0);
    painter->setPen(pen);

    painter->drawLine(QPointF(currentDistanceX, 0.0), QPointF(currentDistanceX, size().height()));
}
