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

    OsmTile osmTile(m_gpxTrack->point(0).lat(), m_gpxTrack->point(0).lon(), 7);

    for (qint64 i = 0; i < m_gpxTrack->pointsCount(); ++i)
    {
        const auto &gpxPoint = m_gpxTrack->point(i);
        points.append(osmTile.latLonToPoint(m_tileImage.width(), m_tileImage.height(), gpxPoint.lat(), gpxPoint.lon()));
    }

    QPen pen(Qt::black);
    pen.setWidthF(2.0);
    painter.setPen(pen);

    if (m_tileImage.isNull())
        requestTile(osmTile);

    painter.drawImage(0, 0, m_tileImage);
    painter.drawPolyline(points.data(), points.count());
}

void GpxMapWidget::requestTile(const OsmTile &tile)
{
    QNetworkRequest request;
    request.setUrl(tile.url());
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) MyOwnBrowserApp/1.0 Mobile Safari/534.3");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, manager, reply] () {
        m_tileImage = QImage::fromData(reply->readAll());

        reply->deleteLater();
        manager->deleteLater();

        repaint();
    });
}
