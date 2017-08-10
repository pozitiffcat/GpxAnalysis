#include "OsmTileManager.h"

#include <QNetworkReply>

OsmTileManager::OsmTileManager(QObject *parent)
    : QObject(parent)
{
}

void OsmTileManager::requestTile(const OsmTile &osmTile)
{
    QNetworkRequest request;
    request.setUrl(osmTile.url());
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) MyOwnBrowserApp/1.0 Mobile Safari/534.3");

    QNetworkReply *reply = m_manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, osmTile, reply] () {
        QImage image = QImage::fromData(reply->readAll());
        m_imageCache.insert(osmTile.url(), image);
        emit imageReady(image);
        reply->deleteLater();
    });
}

bool OsmTileManager::hasImageInCache(const OsmTile &osmTile) const
{
    return m_imageCache.contains(osmTile.url());
}

QImage OsmTileManager::imageFromCache(const OsmTile &osmTile) const
{
    return m_imageCache.value(osmTile.url());
}
