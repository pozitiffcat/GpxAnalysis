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

    void setCurrentDistance(double distance);
    double currentDistance() const;

signals:
    void currentDistanceChangedByMouse();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintData(QPainter *paintDevice);

private:
    double m_currentDistance = 0.0;
    AnalysisData * m_analysisData = nullptr;
};

#endif // ANALYSISWIDGET_H
