#include <cmath>
#include <QtDebug>
#include "model.h"
#include "route.h"
#include "settings.h"
#include "track.h"

const double Model::baseLen = 20037508.34;

/*
Overlay::Overlay(const QString &key, int z):
        myKey(key), myZ(z)
{}
*/
Model::Model(const Settings &settings) :
    myLayer(settings.baseLayers()[0]) , myCenter(settings.center()),
    mySrtmDir(settings.srtmDir()), myZoom(settings.zoom()), myWidth(settings.xExt()),
    myHeight(settings.yExt())
{
    qDebug()<<"lonlat: "<<myCenter;
    QPoint iCenter = Model::lonLat2Tile(myCenter, myZoom);
    qDebug()<<"model center"<<iCenter;
    myX = iCenter.x()-myWidth/2;
    myY = iCenter.y()-myHeight/2;
    connect(&myRoute, SIGNAL(routeChanged()), this, SLOT(updateRoute()));
    connect(&myRoute, SIGNAL(routePointMoved(int)), this, SLOT(moveRoutePoint(int)));
}

void Model::updateSettings(const Settings &settings) {
    mySrtmDir = settings.srtmDir();
    qDeleteAll(mySrtmData);
    mySrtmData.clear();
}

void Model::setCenter(const QPointF &center) {
    myCenter = center;
    QPoint iCenter = Model::lonLat2Tile(myCenter, myZoom);
    myX = iCenter.x() - myWidth/2;
    myY = iCenter.y() - myHeight/2;
    emit mapChanged();
}

void Model::setZoom(int zoom) {
    myZoom = zoom;
    QPoint iCenter = Model::lonLat2Tile(myCenter, myZoom);
    myX = iCenter.x() - myWidth/2;
    myY = iCenter.y() - myHeight/2;
    qDebug()<<"posNew: "<<myX<<", "<<myY;
    emit mapChanged();
}

void Model::zoomIn(const QPointF& center) {
    if (myZoom >= 18) {
        return;
    }
    myZoom++;
    QPoint iCenter = Model::lonLat2Tile(center, myZoom);
    myX = iCenter.x() - myWidth/2;
    myY = iCenter.y() - myHeight/2;
    qDebug()<<"posNew: "<<myX<<", "<<myY;
    emit mapChanged();
}

void Model::zoomOut(const QPointF& center) {
    if (myZoom < 2) {
        return;
    }
    myZoom--;
    QPoint iCenter = Model::lonLat2Tile(center, myZoom);
    myX = iCenter.x() - myWidth/2;
    myY = iCenter.y() - myHeight/2;
    qDebug()<<"posNew: "<<myX<<", "<<myY;
    emit mapChanged();
}

bool Model::isInMap(const QPointF &lonLat) const {
    QPoint p = Model::lonLat2Tile(lonLat, myZoom);
    return p.x() >= myX && p.x() < myX+myWidth && p.y() >= myY && p.y() < myY+myHeight;
}

void Model::changeSize(int north, int east, int south, int west) {
    if (myWidth+west+east < 2 || myHeight+north+south < 2)
        return;
    myX -= west;
    myY -= north;
    myWidth += west+east;
    myHeight += north+south;
    emit mapChanged();
}

void Model::changeOverlays(const QList<Layer>&  overlays) {
    myOverlays.clear();
    foreach(Layer ovl, overlays) {
        myOverlays.append(ovl);
    }
    emit mapChanged();
}

QPixmap *Model::getPixmap(const QString &key) {
    if (myPixmaps.size() > 2000) {
        for (int i = 0; i < 1000; i++) {
            myPixmaps.removeFirst();
        }
    }
    foreach(PixmapEntry e, myPixmaps) {
        if (e.key() == key)
            return e.pixmap();
    }
    return NULL;
}

void Model::savePixmap(const QString &key, QPixmap *pixmap) {
    myPixmaps.append(PixmapEntry(key, pixmap));
}

void Model::setTrack(const Track& track) {
    myTrack = track;
    if (!myTrack.isEmpty()) {
        BoundingBox bb = track.boundingBox();
        myCenter = QPointF(0.5*(bb.p0().x()+bb.p1().x()), 0.5*(bb.p0().y()+bb.p1().y()));
        QPoint iCenter = Model::lonLat2Tile(myCenter, myZoom);
        myX = iCenter.x() - myWidth/2;
        myY = iCenter.y() - myHeight/2;
        myTrack.setPos(0);
    }
    emit mapChanged();
}

QPointF Model::lonLat(const QPointF &mousePos) {
    int x = (int)mousePos.x();
    int y = (int)mousePos.y();
    int ix = myX+x/256;
    int iy = myY+y/256;
    int dx = x % 256;
    int dy = y % 256;
    QPointF p0 = Model::tile2LonLat(QPoint(ix, iy), myZoom);
    QPointF p1 = Model::tile2LonLat(QPoint(ix+1, iy+1), myZoom);
    //qDebug()<<"lonlat "<<p0<<" "<<p1<<" "<<dx<<" "<<dy;
    return QPointF(p0.x()+(p1.x()-p0.x())*dx/256.0,
                   p0.y()+(p1.y()-p0.y())*dy/256.0);
}

QPoint Model::lonLat2Tile (const QPointF &point, int zoom) {
    double lat = point.y()*M_PI/180.0;
    int n = 1<<zoom;
    int x = (int)floor((point.x()+180.0)/360.0*n);
    int y = (int)floor((1.0-log(tan(lat)+1.0/cos(lat))/ M_PI) / 2.0 * n);
    return QPoint(x, y);
}

QPointF Model::lonLat2TileF(const QPointF &point, int zoom) {
    double lat = point.y()*M_PI/180.0;
    int n = 1<<zoom;
    double x = (point.x()+180.0)/360.0*n;
    double y = (1.0-log(tan(lat)+1.0/cos(lat))/ M_PI) / 2.0 * n;
    return QPointF(x, y);
}

const QPointF Model::lonLat2SpherMerc(const QPointF &point) {
    double lat = point.y()*M_PI/180.0;
    double x = (point.x()+180.0)/360.0*baseLen;
    double y = (1.0-log(tan(lat)+1.0/cos(lat))/ M_PI) / 2.0 * baseLen;
    return QPointF(x, y);
}

const QPointF Model::spherMerc2lonLat(const QPointF &point) {
    double x = point.x()*360.0/baseLen - 180.0;
    double y = M_PI-2.0*M_PI*point.y()/baseLen;
    y = 180.0/M_PI * atan(0.5*(exp(y)-exp(-y)));
    return QPointF(x, y);
}

QPoint Model::lonLat2Scene(const QPointF &point) {
    QPointF p = lonLat2TileF(point, myZoom);
    return QPoint((int)round((p.x()-myX)*256), (int)round((p.y()-myY)*256));
}

QPointF Model::tile2LonLat(const QPoint& tile, int zoom) {
    //qDebug()<<"tile2LonLat "<<tile<<" "<<zoom;
    int n = 1<<zoom;
    double x = tile.x()*360.0/n - 180.0;
    double y = M_PI-2.0*M_PI*tile.y()/n;
    y = 180.0/M_PI * atan(0.5*(exp(y)-exp(-y)));
    //qDebug()<<"x: "<<x<<" y: "<<y;
    return QPointF(x, y);
}

double Model::mercUnitsM(const QPointF& pm) {
    QPointF p0(pm.x()-1.0, pm.y()-1.0);
    QPointF p1(pm.x()+1.0, pm.y()+1.0);
    double m = geodist1(p0, p1)*1000.0;
    p0 = lonLat2SpherMerc(p0);
    p1 = lonLat2SpherMerc(p1);
    double dist = hypot(p1.x()-p0.x(), p1.y()-p0.y());
    return m/dist;
}

double Model::geodist0(const QPointF& p0, const QPointF& p1) {
  double lon0, lat0, lon1, lat1;
  double zeta;

  lon0 = p0.x()*M_PI/180.0;
  lat0 = p0.y()*M_PI/180.0;
  lon1 = p1.x()*M_PI/180.0;
  lat1 = p1.y()*M_PI/180.0;
  zeta = sin(lat0)*sin(lat1)+cos(lat0)*cos(lat1)*cos(lon1-lon0);
  if (zeta > 1.0) zeta = 1.0;
  if (zeta < -1.0) zeta = -1.0;
  return acos(zeta)*6370.0;
}

double Model::geodist1(const QPointF& p0, const QPointF& p1) {
  double f = 1.0/298.257223563; // Abplattung
  double a = 6378.137; // radius
  double ff, gg, l;
  double s, c, cosl, sinl, cosf, sinf, cosg, sing;
  double w, d, r, h1, h2;
  double dist;

  if (p0 == p1) return 0.0;
  ff = 0.5*(p0.y()+p1.y())*M_PI/180.0;
  gg = 0.5*(p1.y()-p0.y())*M_PI/180.0;
  l = 0.5*(p1.x()-p0.x())*M_PI/180.0;
  cosl = cos(l)*cos(l);
  sinl = sin(l)*sin(l);
  cosf = cos(ff)*cos(ff);
  sinf = sin(ff)*sin(ff);
  cosg = cos(gg)*cos(gg);
  sing = sin(gg)*sin(gg);
  s = sing*cosl+cosf*sinl;
  c = cosg*cosl+sinf*sinl;
  w = atan(sqrt(s/c));
  d = 2*w*a;
  r = sqrt(s*c)/w;
  h1 = (3*r-1)/(2*c);
  h2 = (3*r+1)/(2*s);
  dist = d*(1+f*h1*sinf*cosg-f*h2*cosf*sing);
  return dist;
}

double Model::geodist0(const GpxPointList &points, int i0, int i1) {
    double sum = 0.0;
    for (int i = i0+1; i <= i1; i++) {
        sum += geodist0(points[i-1].coord(), points[i].coord());
    }
    return sum;
}

double Model::geodist1(const GpxPointList &points, int i0, int i1) {
    double sum = 0.0;
    for (int i = i0+1; i <= i1; i++) {
        sum += geodist1(points[i-1].coord(), points[i].coord());
    }
    return sum;
}

void Model::trackSetNew(const QString &fileName, const QString& name, const GpxPointList &ptl) {
    myTrack.setPoints(ptl);
    myTrack.setFileName(fileName);
    myTrack.setName(name);
    // Set center to the center of track bounding box
    //BoundingBox bb = myTrack.boundingBox();
    //QPointF center(0.5*(bb.p0().x()+bb.p1().x()), 0.5*(bb.p0().y()+bb.p1().y()));
    //qDebug()<<"new center "<<center;
    // Set center to first track point
    setCenter(ptl.at(0).coord());
    emit trackChanged();
}

void Model::setTrackPos(int pos) {
    if (myTrack.isEmpty()) return;
    myTrack.setPos(pos);
    if (!isInMap(myTrack.trackPoint(pos).coord())) setCenter(myTrack.trackPoint(pos).coord());
    emit trackPosChanged(myTrack.pos());
}

void Model::changeTrackPos(int delta) {
    if (myTrack.isEmpty()) return;
    setTrackPos(myTrack.pos()+delta);
}

void Model::setTrackPoint(int pos, const GpxPoint& point) {
    if (myTrack.isEmpty()) return;
    myTrack.setTrackPoint(pos, point);
    emit trackChanged();
}

void Model::insertTrackPoint(int pos, const GpxPoint& point) {
    myTrack.insertTrackPoint(pos, point);
    emit trackChanged();
}

void Model::delTrackPoint(int pos) {
    if (myTrack.isEmpty()) return;
    myTrack.delTrackPoint(pos);
    emit trackChanged();
    myTrack.setPos(pos);
    emit trackPosChanged(myTrack.pos());
}

void Model::delTrackPart(int i0, int i1) {
    if (myTrack.isEmpty()) return;
    myTrack.delTrackPart(i0, i1);
    emit trackChanged();
    myTrack.setPos(i0);
    emit trackPosChanged(i0);
}

void Model::delTrack() {
    myTrack.clear();
    emit trackChanged();
}

void Model::changeTrackPoint(int pos, const QPointF& lonLat) {
    if (myTrack.isEmpty()) return;
    myTrack.setTrackPointPos(pos, lonLat);
    emit trackChanged();
}

void Model::uniqueTrack() {
    if (myTrack.removeDoubles() > 0) emit trackChanged();
}

void Model::saveModifiedTrack(QIODevice *dev, bool addWaypoints, bool isSimple) {
    if (addWaypoints) trackPtr()->writeModifiedXml(dev, waypoints(), isSimple);
    else              trackPtr()->writeModifiedXml(dev, GpxPointList(), isSimple);
}

void Model::changeRoutePoint(int pos, const QPointF &lonLat) {
    if (myRoute.isEmpty()) return;
    double srtm = srtmEle(lonLat);
    myRoute.moveRoutePoint(pos, lonLat, srtm);
    emit routeChanged();
}

void Model::setRoutePoint(int idx, const GpxPoint& p) {
    myRoute.updateRoutePoint(idx, p);
    emit routeChanged();
}

void Model::changeWaypoint(int pos, const QPointF &lonLat) {
    if (myWaypoints.isEmpty()) return;
    myWaypoints[pos].setCoord(lonLat);
    emit waypointsChanged();
}

void Model::routeSetNew(const QString &fileName) {
    Gpx gpx(fileName);
    if (gpx.routePoints().size() == 0) return;
    routeSetNew(fileName, gpx.routeName(), gpx.routePoints());
}

void Model::routeSetNew(const QString &fileName, const QString &name, const GpxPointList &points) {
    myRoute.setFileName(fileName);
    myRoute.setName(name);
    myRoute.setRoutePoints(points);
    setCenter(points.at(0).coord());
    emit routeChanged();
}

void Model::waypointsSetNew(const GpxPointList &points) {
    myWaypoints = points;
    emit waypointsChanged();
}

void Model::addWaypoint(const GpxPoint &p) {
    qDebug()<<"model added waypoint "<<p.name();
    myWaypoints.append(p);
    emit waypointsChanged();
}

void Model::setWaypoint(int idx, const GpxPoint &p) {
    myWaypoints[idx] = p;
    emit waypointsChanged();
}

void Model::delWaypoint(int idx) {
    myWaypoints.removeAt(idx);
    emit waypointsChanged();
}

const SrtmEntry *Model::srtmEntry(int lon0, int lat0) {
    for (int i = 0; i < mySrtmData.size(); i++) {
        if (mySrtmData[i]->lon0() == lon0 && mySrtmData[i]->lat0() == lat0) return mySrtmData[i];
    }
    if (mySrtmData.size() > 100) {
        SrtmEntry *entry = mySrtmData.last();
        mySrtmData.removeLast();
        delete entry;
    }
    SrtmEntry *entry = new SrtmEntry(lon0, lat0, mySrtmDir);
    mySrtmData.insert(0, entry);
    return mySrtmData[0];
}

int Model::srtmEle(const QPointF& coord) {
    int lon0 = int(floor(coord.x()));
    int lat0 = int(floor(coord.y()));
    const SrtmEntry* srtm = srtmEntry(lon0, lat0);
    return srtm->ele(coord.x(), coord.y());
}
