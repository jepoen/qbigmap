#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPointF>
#include <QString>
#include "gpx.h"
#include "route.h"
#include "settings.h"
#include "srtm.h"
#include "track.h"

class QPixmap;
class QImage;
class Route;

class Model : public QObject
{
    Q_OBJECT
private:
    Layer myLayer;
    QPointF myCenter;
    bool myUseSrtm;
    QString mySrtmDir;
    QString myCacheDir;
    int myX;
    int myY;
    int myZoom;
    int myWidth;
    int myHeight;
    QList<Layer> myOverlays;
    //QList<PixmapEntry> myPixmaps;
    QMap<QString, QString> myTileCache;
    int myTileCacheNr;
    QList<SrtmEntry*> mySrtmData;
    Track myTrack;
    GpxPointList myWaypoints;
    Route myRoute;

public:
    static const double baseLen;
    Model(const Settings& settings);
    const QPointF& center() const { return myCenter; }
    void setCenter(const QPointF& center);
    void updateSettings(const Settings& settings);
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
    bool isInMap(const QPointF& lonLat) const;
    void zoomIn(const QPointF& center);
    void zoomOut(const QPointF& center);
    void setSize(const BoundingBox& bb);
    void changeSize(int nord, int east, int south, int west);
    void changeOverlays(const QList<Layer>& overlays);
    QList<Layer>* overlays() { return &myOverlays; }
    void savePixmap(const QString& key, QPixmap pixmap);
    void clearCache();
    QPixmap getPixmap(const QString &key);
    const SrtmEntry* srtmEntry(int lon0, int lat0);
    int srtmEle(const QPointF& coord);
    QPointF lonLat(const QPointF& mousePos);
    QPoint lonLat2Scene(const QPointF& point);
    static const QPointF lonLat2SpherMerc(const QPointF &point);
    static const QPointF spherMerc2lonLat(const QPointF &point);
    const Track& track() const { return myTrack; }
    Track* trackPtr() { return &myTrack; }
    bool isEmptyTrack() const { return myTrack.trackPoints().size() == 0; }
    void trackSetNew(const QString& fileName, const QString &name, const GpxPointList& ptl);
    void setTrack(const Track &track);
    int trackPos() const { return myTrack.pos(); }
    void setTrackPos(int pos);
    void changeTrackPos(int delta);
    void setTrackPoint(int pos, const GpxPoint &point);
    void insertTrackPoint(int pos, const GpxPoint &point);
    void delTrackPoint(int pos);
    void delTrackPart(int i0, int i1);
    void delTrack();
    void changeTrackPoint(int pos, const QPointF& lonLat);
    void uniqueTrack();
    void trackAddSrtmEle();
    void saveModifiedTrack(QIODevice *dev, bool addWaypoints, bool isSimple=false);
    const Route& route() const { return myRoute; }
    Route *routePtr() { return &myRoute; }
    void routeSetNew(const QString& fileName);
    void routeSetNew(const QString& fileName, const QString& name, const GpxPointList& points);
    void changeRoutePoint(int pos, const QPointF& lonLat);
    void setRoutePoint(int idx, const GpxPoint& p);
    void routeAddSrtmEle();
    void waypointsSetNew(const GpxPointList& points);
    void changeWaypoint(int pos, const QPointF& lonLat);
    const GpxPointList& waypoints() const { return myWaypoints; }
    GpxPointList *wptPtr() { return &myWaypoints; }
    void setWaypoint(int idx, const GpxPoint& p);
    void addWaypoint(const GpxPoint &p);
    void delWaypoint(int pos);
    static QPoint lonLat2Tile (const QPointF& point, int z);
    static QPointF lonLat2TileF (const QPointF& point, int z);
    static QPointF tile2LonLat(const QPoint& tile, int z);
    /**
      * dist/Meter = mercUnitsM()*mercDist;
      */
    static double mercUnitsM(const QPointF& center);
    static double geodist0(const QPointF& p0, const QPointF& p1);
    static double geodist1(const QPointF& p0, const QPointF& p1);
    static double geodist0(const GpxPointList& points, int i0, int i1);
    static double geodist1(const GpxPointList& points, int i0, int i1);

 signals:
    void mapChanged();
    void trackChanged();
    void trackPosChanged(int);
    void routeChanged();
    void routePointMoved(int);
    void waypointsChanged();
private slots:
    void updateRoute() { emit routeChanged(); }
    void moveRoutePoint(int idx) { emit routePointMoved(idx); }
};


#endif // MODEL_H
