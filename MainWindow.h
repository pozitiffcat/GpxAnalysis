#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "AnalysisWidget.h"

#include "GpxTrack.h"
#include "SpeedAnalysisData.h"
#include "ElevationAnalysisData.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void loadRecentMenuFromSettings();
    bool appendRecentMenuToSettings(const QString &fileName);

    void openGpx();
    void openGpxByPath(const QString &fileName);

private:
    Ui::MainWindow *ui;
    AnalysisWidget *m_elevationAnalysisWidget;
    AnalysisWidget *m_speedAnalysisWidget;
    QSharedPointer<GpxTrack> m_gpxTrack;
    QSharedPointer<SpeedAnalysisData> m_speedAnalysisData;
    QSharedPointer<ElevationAnalysisData> m_elevationAnalysisData;
};

#endif // MAINWINDOW_H
