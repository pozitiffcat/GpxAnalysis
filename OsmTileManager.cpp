#include "OsmTileManager.h"

#include <QNetworkReply>

OsmTileManager::OsmTileManager(QObject *parent)
    : QObject(parent)
{
}

void OsmTileManager::requestImageTileList(const QList<OsmTile> osmTileList)
{
    for (const auto &osmTile : osmTileList)
    {
        if (!m_osmTileList.contains(osmTile) && !m_imageCache.contains(osmTile.url()))
            m_osmTileList.append(osmTile);
    }

    if (!m_osmTileList.isEmpty() && !m_requestInProgress)
        requestCurrent();
}

void OsmTileManager::requestCurrent()
{
    m_requestInProgress = true;

    QNetworkRequest request;
    request.setUrl(m_osmTileList.first().url());
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) MyOwnBrowserApp/1.0 Mobile Safari/534.3");

    qDebug() << "Request OSM image" << request.url();

    QNetworkReply *reply = m_manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, reply] () {
        qDebug() << "Respond OSM image" << reply->request().url();

        QImage image = QImage::fromData(reply->readAll());
        m_imageCache.insert(m_osmTileList.first().url(), image);

        m_osmTileList.removeFirst();
        reply->deleteLater();

        if (!m_osmTileList.isEmpty())
        {
            requestCurrent();
        }
        else
        {
            m_requestInProgress = false;
            emit imageTileListReady();
        }
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
