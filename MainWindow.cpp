#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString fileName = "../1117025335.gpx";
    QString fileName = "../ors-export-linestring.gpx";

    m_gpxTrack.parse(fileName);
    setWindowTitle(QString("%1 [%2]").arg(qApp->applicationName(), fileName));
    ui->elevationGainLabel->setText(tr("%1 meters").arg(m_gpxTrack.elevationGain()));

    m_speedAnalysisData.reset(new SpeedAnalysisData(&m_gpxTrack));
    m_elevationAnalysisData.reset(new ElevationAnalysisData(&m_gpxTrack));
    ui->gpxAnalysisWidget->addData(m_speedAnalysisData.data());
    ui->gpxAnalysisWidget->addData(m_elevationAnalysisData.data());
}

MainWindow::~MainWindow()
{
    delete ui;
}
