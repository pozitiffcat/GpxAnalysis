#ifndef OSMTILEMANAGER_H
#define OSMTILEMANAGER_H

#include <QImage>
#include <QNetworkAccessManager>

#include "OsmTile.h"

class OsmTileManager : public QObject
{
    Q_OBJECT

public:
    explicit OsmTileManager(QObject *parent = nullptr);

    void requestTile(const OsmTile &osmTile);
    bool hasImageInCache(const OsmTile &osmTile) const;
    bool hasImageInRequest(const OsmTile &osmTile) const;
    QImage imageFromCache(const OsmTile &osmTile) const;

signals:
    void imageReady(const QImage &image);

private:
    QNetworkAccessManager m_manager;
    QHash<QUrl, QImage> m_imageCache;
    QSet<QUrl> m_imageInRequest;
};

#endif // OSMTILEMANAGER_H
