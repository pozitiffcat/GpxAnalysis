#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>

#include "AnalysisData.h"

class AnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisWidget(QWidget *parent = nullptr);

    void setAnalysisData(AnalysisData *data);
    void setValueMeasureLabel(const QString &label);

    void setCurrentDistance(double distance);
    double currentDistance() const;

signals:
    void currentDistanceChangedByMouse(double distance);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintData(QPainter *painter, const QRectF &dataRect);
    void paintCurrent(QPainter *painter, const QRectF &dataRect);
    void paintHScales(QPainter *painter, const QRectF &dataRect);
    void paintVScales(QPainter *painter, const QRectF &dataRect);

private:
    double m_currentDistance = 0.0;
    AnalysisData * m_analysisData = nullptr;
    QString m_valueMeasureLabel;
};

#endif // ANALYSISWIDGET_H
