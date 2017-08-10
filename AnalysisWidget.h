#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>

#include "AnalysisData.h"

class AnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisWidget(QWidget *parent = nullptr);

    void addData(AnalysisData *data);
    void clearDataSet();

    double currentDistance() const;

signals:
    void currentDistanceChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintData(QPainter *paintDevice, AnalysisData *data, const QColor &color);

private:
    double m_currentDistance = 0.0;
    QList<AnalysisData *> m_gpxAnalysisDataList;
};

#endif // ANALYSISWIDGET_H
