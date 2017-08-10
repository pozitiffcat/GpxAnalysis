#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GpxTrack.h"
#include "OsmMapWidget.h"
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

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void updateWidgetsGeometry();

    void loadRecentMenuFromSettings();
    bool appendRecentMenuToSettings(const QString &fileName);

    void openGpx();
    void openGpxByPath(const QString &fileName);

private:
    Ui::MainWindow *ui;
    QSharedPointer<GpxTrack> m_gpxTrack;
    QSharedPointer<ElevationAnalysisData> m_elevationAnalysisData;
};

#endif // MAINWINDOW_H
