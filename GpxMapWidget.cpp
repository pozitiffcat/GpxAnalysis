#include "GpxMapWidget.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPainter>

#include <math.h>

GpxMapWidget::GpxMapWidget(QWidget *parent)
    : QWidget(parent)
{
}

void GpxMapWidget::setGpxTrack(GpxTrack *gpxTrack)
{
    m_gpxTrack = gpxTrack;
    repaint();
}

QSize GpxMapWidget::sizeHint() const
{
    return QSize(256, 256);
}

void GpxMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_gpxTrack->pointsCount() == 0)
        return;

    QPainter painter(this);

    QVector<QPointF> points;

    for (qint64 i = 0; i < m_gpxTrack->pointsCount(); ++i)
    {
        const auto &gpxPoint = m_gpxTrack->point(i);

        const double mercN = log(tan((M_PI / 4.0) + (degToRad(gpxPoint.lat()) / 2.0)));
        const double x = (gpxPoint.lon() + 180.0) * (256.0 / 360.0);
        const double y = (256.0 / 2.0) - (256.0 * mercN / (2.0 * M_PI));

        qDebug() << "x" << x << "y" << y;

        points.append(QPointF(x, y));
    }

    QPen pen(Qt::black);
    pen.setWidthF(2.0);
    painter.setPen(pen);

    if (m_tile.isNull())
        requestTile();

    painter.drawImage(0, 0, m_tile);
    painter.drawPolyline(points.data(), points.count());
}

double GpxMapWidget::radToDeg(double r) const
{
    return r * (180.0 / M_PI);
}

double GpxMapWidget::degToRad(double d) const
{
    return d / (180.0 / M_PI);
}

void GpxMapWidget::requestTile()
{
    QNetworkRequest request;
    request.setUrl(QUrl::fromUserInput("http://a.tile.openstreetmap.org/0/0/0.png"));
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) MyOwnBrowserApp/1.0 Mobile Safari/534.3");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, manager, reply] () {
        m_tile = QImage::fromData(reply->readAll());

        reply->deleteLater();
        manager->deleteLater();

        repaint();
    });
}
