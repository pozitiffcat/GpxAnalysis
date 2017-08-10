#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMouseEvent>

#include <QFuture>
#include <QtConcurrent>

#include <QSettings>
#include <QFileDialog>
#include <QProgressDialog>
#include <QGraphicsEffect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadRecentMenuFromSettings();

    QGraphicsDropShadowEffect *textShadowEffect = new QGraphicsDropShadowEffect(this);
    textShadowEffect->setBlurRadius(6);
    textShadowEffect->setColor(Qt::black);
    textShadowEffect->setXOffset(0);
    textShadowEffect->setYOffset(0);

    ui->overlayFrame->setGraphicsEffect(textShadowEffect);

    QGraphicsDropShadowEffect *frameShadowEffect = new QGraphicsDropShadowEffect(this);
    frameShadowEffect->setBlurRadius(20);
    frameShadowEffect->setColor(Qt::darkGray);
    frameShadowEffect->setXOffset(0);
    frameShadowEffect->setYOffset(0);

    ui->analysisFrame->setGraphicsEffect(frameShadowEffect);

    ui->analysisWidget->setValueMeasureLabel(tr("m"));

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openGpx);
    connect(ui->analysisWidget, &AnalysisWidget::currentDistanceChangedByMouse, ui->osmMapWidget, &OsmMapWidget::setCurrentDistance);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    updateWidgetsGeometry();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateWidgetsGeometry();
}

void MainWindow::updateWidgetsGeometry()
{
    QRect osmMapGeometry = ui->osmMapWidget->geometry();
    osmMapGeometry.setTopLeft(QPoint(0, 0));
    osmMapGeometry.setSize(ui->baseWidget->size());
    ui->osmMapWidget->setGeometry(osmMapGeometry);

    QRect analysisGeometry = ui->analysisFrame->geometry();
    analysisGeometry.moveTo(QPoint(ui->baseWidget->size().width() - analysisGeometry.width() - 16, ui->baseWidget->size().height() - analysisGeometry.height() - 16));
    ui->analysisFrame->setGeometry(analysisGeometry);
}

void MainWindow::loadRecentMenuFromSettings()
{
    ui->menuRecent->clear();

    QSettings settings;

    QStringList recentList = settings.value("Recent").toStringList();
    for (const QString &fileName : recentList)
    {
        ui->menuRecent->addAction(fileName, [this, fileName]() {
            openGpxByPath(fileName);
        });
    }
}

bool MainWindow::appendRecentMenuToSettings(const QString &fileName)
{
    QSettings settings;

    QStringList recentList = settings.value("Recent").toStringList();
    if (recentList.contains(fileName))
        return false;

    if (recentList.count() == 10)
        recentList.removeFirst();

    recentList.append(fileName);
    settings.setValue("Recent", recentList);
    return true;
}

void MainWindow::openGpx()
{
    QFileDialog *openDialog = new QFileDialog(this);

    connect(openDialog, &QFileDialog::finished, openDialog, &QFileDialog::deleteLater);

    connect(openDialog, &QFileDialog::accepted, [this, openDialog]() {
        openGpxByPath(openDialog->selectedFiles().first());
    });

    openDialog->show();
}

void MainWindow::openGpxByPath(const QString &fileName)
{
    ui->osmMapWidget->setGpxTrack(nullptr);
    ui->analysisWidget->setAnalysisData(nullptr);

    QFuture<void> future = QtConcurrent::run(QThreadPool::globalInstance(), [this, fileName]() {
        m_gpxTrack.reset(new GpxTrack(fileName));
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);

    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);

    connect(watcher, &QFutureWatcher<void>::finished, [this, fileName]() {
        setWindowTitle(QString("%1 [%2]").arg(qApp->applicationName(), fileName));

        ui->osmMapWidget->setGpxTrack(m_gpxTrack.data());
        m_elevationAnalysisData.reset(new ElevationAnalysisData(m_gpxTrack.data()));
        ui->analysisWidget->setAnalysisData(m_elevationAnalysisData.data());

        ui->distanceLabel->setText(tr("%1 km").arg(m_gpxTrack->distance() / 1000.0, 0, 'f', 1));
        ui->elevationGainLabel->setText(tr("%1 m").arg(m_gpxTrack->elevationGain(), 0, 'f', 1));

        if (appendRecentMenuToSettings(fileName))
            loadRecentMenuFromSettings();
    });

    QProgressDialog *progressDialog = new QProgressDialog(this);
    progressDialog->setLabelText(tr("Please wait..."));
    progressDialog->setRange(0, 0);

    connect(watcher, &QFutureWatcher<void>::finished, progressDialog, &QProgressDialog::deleteLater);

    progressDialog->show();

    watcher->setFuture(future);
}
