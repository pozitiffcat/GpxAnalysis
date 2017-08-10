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

    void requestImageTileList(const QList<OsmTile> osmTileList);
    void requestCurrent();

    bool hasImageInCache(const OsmTile &osmTile) const;
    QImage imageFromCache(const OsmTile &osmTile) const;

signals:
    void imageTileListReady();

private:
    QNetworkAccessManager m_manager;
    QHash<QUrl, QImage> m_imageCache;
    QList<OsmTile> m_osmTileList;
    bool m_requestInProgress = false;
};

#endif // OSMTILEMANAGER_H
