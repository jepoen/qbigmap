#ifndef OSMMAP_H
#define OSMMAP_H

#include <QPointF>
#include <QString>

class MapIconList;
class QPixMap;

class OsmMap
{
private:
    QPointF myCenter;
    int myZoom;

    QString pixEntry(const QString& name, const QString& fileName, const QPixmap& pix);
public:
    OsmMap(const QPointF& center, int zoom):
        myCenter(center), myZoom(zoom)
    {}
    bool writeTrackFile(const QString& osmFileName, const QString& trackFileName, const QString &title, const MapIconList &mapIconList);
};

#endif // OSMMAP_H
