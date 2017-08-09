#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gpxTrack.parse("../1117025335.gpx");
}

MainWindow::~MainWindow()
{
    delete ui;
}
