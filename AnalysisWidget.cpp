#include "AnalysisWidget.h"

#include <QPainter>
#include <QMouseEvent>

namespace
{

const int DataMarginLeft = 70;
const int DataMarginBottom = 20;

}

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

void AnalysisWidget::setValueMeasureLabel(const QString &label)
{
    m_valueMeasureLabel = label;
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
    m_currentDistance = qMax(0, event->x() - DataMarginLeft) * distance / (size().width() - DataMarginLeft);

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

    QRectF dataRect;
    dataRect.setLeft(DataMarginLeft);
    dataRect.setTop(0);
    dataRect.setRight(size().width());
    dataRect.setBottom(size().height() - DataMarginBottom);

    paintData(&painter, dataRect);
    paintCurrent(&painter, dataRect);
    paintHScales(&painter, dataRect);
    paintVScales(&painter, dataRect);
}

void AnalysisWidget::paintData(QPainter *painter, const QRectF &dataRect)
{
    painter->save();
    painter->translate(dataRect.topLeft());

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
        points.append(QPointF(distanceTo * dataRect.size().width() / distance, dataRect.size().height() - (value - min) * dataRect.size().height() / (max - min)));
    }

    QPen pen(Qt::darkCyan);
    pen.setWidth(1.0);
    painter->setPen(pen);

    painter->drawPolyline(points.data(), points.count());

    painter->restore();
}

void AnalysisWidget::paintCurrent(QPainter *painter, const QRectF &dataRect)
{
    painter->save();
    painter->translate(dataRect.topLeft());
    painter->setRenderHint(QPainter::Antialiasing, false);

    double distance = m_analysisData->distance();
    double currentDistanceX = m_currentDistance * dataRect.size().width() / distance;

    QPen pen(Qt::darkMagenta);
    pen.setWidth(1.0);
    painter->setPen(pen);

    painter->drawLine(QPointF(currentDistanceX, 0.0), QPointF(currentDistanceX, dataRect.size().height()));

    painter->restore();
}

void AnalysisWidget::paintHScales(QPainter *painter, const QRectF &dataRect)
{
    painter->save();
    painter->translate(dataRect.topLeft());
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setFont(QFont("Sans", 8));

    QPen mainPen(Qt::darkGray);
    mainPen.setWidth(1.0);

    QPen secondaryPen(Qt::lightGray);
    secondaryPen.setWidth(1.0);

    const int meterLineSize = 75;
    const int meterLinesCount = dataRect.size().width() / meterLineSize;

    for (int i = 0; i <= meterLinesCount; ++i)
    {
        const double m = i * m_analysisData->distance() / (meterLinesCount + 1);

        const int textWidth = 100;
        const QString text = tr("%1 km").arg(m / 1000.0, 0, 'f', 1);

        painter->setPen(mainPen);
        painter->drawText(QRectF(i * meterLineSize - textWidth / 2.0, 4 + dataRect.size().height(), textWidth, 20),
                          text, QTextOption(Qt::AlignHCenter));

        painter->setPen(secondaryPen);
        painter->drawLine(QPointF(i * meterLineSize, 0.0), QPointF(i * meterLineSize, dataRect.size().height()));
    }

    painter->setPen(mainPen);
    painter->drawLine(QPointF(0.0, 0.0), QPointF(0.0, dataRect.size().height()));

    painter->restore();
}

void AnalysisWidget::paintVScales(QPainter *painter, const QRectF &dataRect)
{
    double min = 99999.9;
    double max = -99999.9;

    for (qint64 i = 0; i < m_analysisData->count(); ++i)
    {
        const double value = m_analysisData->valueOnPosition(i);
        min = qMin(min, value);
        max = qMax(max, value);
    }

    painter->save();
    painter->translate(dataRect.topLeft());
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setFont(QFont("Sans", 8));

    QPen mainPen(Qt::darkGray);
    mainPen.setWidth(1.0);

    QPen secondaryPen(Qt::lightGray);
    secondaryPen.setWidth(1.0);

    const int valueLineSize = 25;
    const int valueLinesCount = dataRect.size().height() / valueLineSize;

    for (int i = 1; i <= valueLinesCount; ++i)
    {
        const double v = min + (valueLinesCount - i + 1) * (max - min) / (valueLinesCount + 1);

        const int textHeight = 20;
        const QString text = QString("%1 %2").arg(v, 0, 'f', 1).arg(m_valueMeasureLabel);

        painter->setPen(mainPen);
        painter->drawText(QRectF(-104, i * valueLineSize - (textHeight / 2.0), 100, textHeight),
                          text, QTextOption(Qt::AlignRight | Qt::AlignVCenter));

        painter->setPen(secondaryPen);
        painter->drawLine(QPointF(0.0, i * valueLineSize), QPointF(dataRect.size().width(), i * valueLineSize));
    }

    painter->setPen(mainPen);
    painter->drawLine(QPointF(0.0, dataRect.size().height()), QPointF(dataRect.size().width(), dataRect.size().height()));

    painter->restore();
}
