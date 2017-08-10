#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMouseEvent>

#include <QFuture>
#include <QtConcurrent>

#include <QSettings>
#include <QFileDialog>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadRecentMenuFromSettings();

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openGpx);
    connect(ui->analysisWidget, &AnalysisWidget::currentDistanceChangedByMouse, ui->osmWidget, &OsmWidget::setCurrentDistance);
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
    QRect osmGeometry = ui->osmWidget->geometry();
    osmGeometry.setTopLeft(QPoint(0, 0));
    osmGeometry.setSize(ui->baseWidget->size());
    ui->osmWidget->setGeometry(osmGeometry);

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
    QFuture<void> future = QtConcurrent::run(QThreadPool::globalInstance(), [this, fileName]() {
        m_gpxTrack.reset(new GpxTrack(fileName));
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);

    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);

    connect(watcher, &QFutureWatcher<void>::finished, [this, fileName]() {
        setWindowTitle(QString("%1 [%2]").arg(qApp->applicationName(), fileName));

        ui->osmWidget->setGpxTrack(m_gpxTrack.data());
        m_elevationAnalysisData.reset(new ElevationAnalysisData(m_gpxTrack.data()));
        ui->analysisWidget->setAnalysisData(m_elevationAnalysisData.data());

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
