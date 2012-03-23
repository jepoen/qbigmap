#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QString>
#include "settings.h"
#include "track.h"

class QPixmap;
class QImage;
class Route;

class PixmapEntry
{
private:
    QString myKey;
    QPixmap *myPixmap;
public:
    PixmapEntry(const QString& key, QPixmap *pixmap) :
            myKey(key), myPixmap(pixmap)
    {}
    const QString& key() { return myKey; }
    QPixmap *pixmap() { return myPixmap; }
};

class Model : public QObject
{
    Q_OBJECT
private:
    Layer myLayer;
    QPointF myCenter;
    int myX;
    int myY;
    int myZoom;
    int myWidth;
    int myHeight;
    QList<Layer> myOverlays;
    QList<PixmapEntry> myPixmaps;
    Track *myTrack;
    Route *myRoute;

public:
    static const double baseLen;
    Model(const Layer& layer, const QPointF& center);
    const QPointF& center() const { return myCenter; }
    void setCenter(const QPointF& center) { myCenter = center; }
    int x() const { return myX; }
    void setX(int x) { myX = x; }
    int y() const { return myY; }
    void setY(int y) { myY = y; }
    int width() const { return myWidth; }
    int height() const { return myHeight; }
    int zoom() const { return myZoom; }
    void setZoom(int zoom);
    const Layer& layer() { return myLayer; }
    void setLayer(const Layer& layer) { myLayer = layer; emit mapChanged(); }
    void zoomIn(const QPointF& center);
    void zoomOut(const QPointF& center);
    void changeSize(int nord, int east, int south, int west);
    void changeOverlays(const QList<Layer>& overlays);
    QList<Layer>* overlays() { return &myOverlays; }
    QList<PixmapEntry> pixmaps() { return myPixmaps; }
    void savePixmap(const QString& key, QPixmap *pixmap);
    QPixmap *getPixmap(const QString &key);
    QPointF lonLat(const QPointF& mousePos);
    QPoint lonLat2Scene(const QPointF& point);
    static const QPointF lonLat2SpherMerc(const QPointF &point);
    static const QPointF spherMerc2lonLat(const QPointF &point);
    Track *track() const { return myTrack; }
    void setTrack(Track *track);
    void setTrackPos(int pos);
    void changeTrackPos(int delta);
    void setTrackPoint(int pos, const ExtTrackPoint point);
    void delTrackPoint(int pos);
    Route *route() const { return myRoute; }
    static QPoint lonLat2Tile (const QPointF& point, int z);
    static QPointF lonLat2TileF (const QPointF& point, int z);
    static QPointF tile2LonLat(const QPoint& tile, int z);
    static double geodist0(const QPointF& p0, const QPointF& p1);
    static double geodist1(const QPointF& p0, const QPointF& p1);

 signals:
    void mapChanged();
    void trackChanged();
    void trackPosChanged(int);
    void routeChanged();
    void routePointMoved(int);
private slots:
    void updateRoute() { emit routeChanged(); }
    void moveRoutePoint(int idx) { emit routePointMoved(idx); }
};


#endif // MODEL_H
