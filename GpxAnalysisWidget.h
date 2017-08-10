#ifndef GPXANALYSISWIDGET_H
#define GPXANALYSISWIDGET_H

#include <QWidget>

#include "GpxAnalysisData.h"

class GpxAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GpxAnalysisWidget(QWidget *parent = nullptr);

    void addData(GpxAnalysisData *data);
    double currentDistance() const;

signals:
    void currentDistanceChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    void paintData(QPainter *paintDevice, GpxAnalysisData *data, const QColor &color);

private:
    double m_currentDistance = 0.0;
    QList<GpxAnalysisData *> m_gpxAnalysisDataList;
};

#endif // GPXANALYSISWIDGET_H
