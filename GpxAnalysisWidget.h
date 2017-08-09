#ifndef GPXANALYSISWIDGET_H
#define GPXANALYSISWIDGET_H

#include <QWidget>

#include "GpxAnalysisData.h"

class GpxAnalysisWidget : public QWidget
{
public:
    explicit GpxAnalysisWidget(QWidget *parent = nullptr);

    void addData(GpxAnalysisData *data);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void paintData(QPainter *paintDevice, GpxAnalysisData *data, const QColor &color);

private:
    QList<GpxAnalysisData *> m_gpxAnalysisDataList;
};

#endif // GPXANALYSISWIDGET_H
