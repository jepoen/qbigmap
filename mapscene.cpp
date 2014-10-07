#include <QtGui>
#include <QtDebug>
#include "httpget.h"
#include "mapscene.h"
#include "route.h"
#include "track.h"

TrackItem::TrackItem(const QPolygonF &points, QGraphicsItem *parent) :
        QGraphicsItem(parent), myPoints(points), myColor(qRgba(0, 255, 255, 20))
{
    setFlags(ItemSendsGeometryChanges|ItemSendsScenePositionChanges);
    setZValue(5);
}

QRectF TrackItem::boundingRect() const {
    qDebug()<<"TrackItem::boundingRect"<<myPoints.boundingRect();
    return myPoints.boundingRect();
}

QPainterPath TrackItem::shape() const {
    QPainterPath path;
    path.moveTo(myPoints[0]);
    for (int i = 1; i < myPoints.size(); i++)
        path.lineTo(myPoints[i]);
    path.addPath(path.toReversed());
    return path;
}

void TrackItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qDebug()<<"TrackItem::paint";
    QPen pen;
    QBrush brush(myColor);
    pen.setWidth(2);
    pen.setBrush(brush);
    painter->setPen(pen);
    painter->drawPolyline(myPoints);
}

void TrackItem::setPoints(const QPolygonF &points) {
    prepareGeometryChange();
    myPoints = points;
    update();
}

TrackPointItem::TrackPointItem(const QPointF &point, const QString& sym, QGraphicsItem *parent) :
        QGraphicsItem(parent),
    myPoint(point), mySym(sym != "")
{
    setZValue(10);
}

QRectF TrackPointItem::boundingRect() const {
    //qDebug()<<"routePointItem bb: "<<QRectF(myPoint.x()-3, myPoint.y()-3, 7, 7);
    return QRectF(myPoint.x()-5, myPoint.y()-5, 11, 11);
}

QPainterPath TrackPointItem::shape() const {
    QPainterPath path;
    path.addEllipse(myPoint, 3, 3);
    return path;
}

void TrackPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path = shape();
    QRgb col = qRgba(0, 0, 255, 20);
    if (mySym) {
        col = qRgba(255, 255, 0, 20);
    }
    painter->fillPath(path, QBrush(col));
    painter->strokePath(path, QPen(Qt::black));
}

void TrackPointItem::setPoint(const QPointF &point) {
    prepareGeometryChange();
    myPoint = point;
    update();
}

void TrackPointItem::setSym(const QString& sym) {
    mySym = (sym != "");
    update();
}

WaypointItem::WaypointItem(const QString &text, QGraphicsItem *parent) :
        QGraphicsItem(parent), myText(text)
{
}

QRectF WaypointItem::boundingRect() const {
    QFont font("FreeSans", 8);
    QFontMetrics fm(font);
    QRectF r = fm.boundingRect(myText);
    r.setX(r.x()-3);
    r.setY(r.y()-3);
    r.setWidth(r.width()+10);
    r.setHeight(r.height()+5);
    //qDebug()<<"Waypoint r: "<<r;
    return r;
}

QPainterPath WaypointItem::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void WaypointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen(Qt::magenta);
    pen.setWidth(1);
    painter->setPen(pen);
    QFont font("Helvetica", 8);
    painter->setFont(font);
    painter->drawEllipse(-3, -3, 6, 6);
    /*
    QFontMetrics fm(font);
    QRectF r = fm.boundingRect(myText);
    r.setX(r.x()+5);
    r.setY(r.y()+5);
    */
    painter->drawText(QPointF(5, 5), myText);
}

TrackPosItem::TrackPosItem(const QPointF& pos, QGraphicsItem *parent) :
    QGraphicsItem(parent), myPos(QPointF(0,0))
{
    //setFlags(ItemSendsGeometryChanges|ItemSendsScenePositionChanges);
    setPos(pos);
}

QRectF TrackPosItem::boundingRect() const {
    //qDebug()<<"TrackPosItem::boundingRect";
    return QRectF(myPos.x()-18, myPos.y()-18, 36, 36);
}

QPainterPath TrackPosItem::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void TrackPosItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen pen(Qt::magenta);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(QRectF(myPos.x()-16, myPos.y()-16, 32, 32));
    painter->drawLine(myPos.x()-16, myPos.y()-16, myPos.x()+16, myPos.y()+16);
    painter->drawLine(myPos.x()-16, myPos.y()+16, myPos.x()+16, myPos.y()-16);
}

//void TrackPosItem::setPos(const QPointF &pos) {
//    //prepareGeometryChange();
//    qDebug()<<"setPos "<<pos;
//    //setPos(pos);
//    //myPos = pos;
//}

RouteItem::RouteItem(const QPolygonF &points, QGraphicsItem *parent) :
        QGraphicsItem(parent), myPoints(points)
{
    setZValue(5);
}

QRectF RouteItem::boundingRect() const {
    return myPoints.boundingRect();
}

QPainterPath RouteItem::shape() const {
    QPainterPath path;
    path.moveTo(myPoints[0]);
    for (int i = 1; i < myPoints.size(); i++)
        path.lineTo(myPoints[i]);
    path.addPath(path.toReversed());
    return path;
}

void RouteItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qDebug()<<"RouteItem::paint";
    QPen pen;
    QBrush brush(qRgba(255, 0, 255, 20));
    pen.setWidth(2);
    pen.setBrush(brush);
    painter->setPen(pen);
    painter->drawPolyline(myPoints);
}

void RouteItem::setPoints(const QPolygonF &points) {
    prepareGeometryChange();
    myPoints = points;
    update();
}

RoutePointItem::RoutePointItem(const QPointF &point, const QString& sym, QGraphicsItem *parent) :
        QGraphicsItem(parent),
    myPoint(point), mySym(sym != "")
{
    setZValue(10);
}

QRectF RoutePointItem::boundingRect() const {
    //qDebug()<<"routePointItem bb: "<<QRectF(myPoint.x()-3, myPoint.y()-3, 7, 7);
    return QRectF(myPoint.x()-5, myPoint.y()-5, 11, 11);
}

QPainterPath RoutePointItem::shape() const {
    QPainterPath path;
    path.addEllipse(myPoint, 3, 3);
    return path;
}

void RoutePointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path = shape();
    QRgb col = qRgba(255, 0, 255, 20);
    if (mySym) {
        col = qRgba(0, 255, 255, 20);
    }
    painter->fillPath(path, QBrush(col));
    painter->strokePath(path, QPen(Qt::black));
}

void RoutePointItem::setPoint(const QPointF &point) {
    prepareGeometryChange();
    myPoint = point;
    update();
}

void RoutePointItem::setSym(const QString& sym) {
    mySym = (sym != "");
    update();
}

PhotoItem::PhotoItem(const QPointF &point, QGraphicsItem *parent) :
        QGraphicsItem(parent),
    myPoint(point)
{
    setZValue(10);
    myPath = createPath();
    myBoundingRect = myPath.boundingRect().adjusted(1, 1, 1, 1);
}

QPainterPath PhotoItem::createPath() {
    QPainterPath path;
    path.addRect(myPoint.x()-10, myPoint.y()-5, 20, 10);
    path.moveTo(myPoint.x()-10, myPoint.y()-5);
    path.lineTo(myPoint.x()+10, myPoint.y()+5);
    path.moveTo(myPoint.x()-10, myPoint.y()+5);
    path.lineTo(myPoint.x()+10, myPoint.y()-5);
    return path;
}

void PhotoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) {
    painter->strokePath(myPath, QPen(QBrush(Qt::blue), 1));
}

void PhotoItem::setPoint(const QPointF &point) {
    prepareGeometryChange();
    myPoint = point;
    myPath = createPath();
}

MapScene::MapScene(Model *model, QObject *parent) :
        QGraphicsScene(parent),
        myModel(model),
        myTrackItem(0),
        myTrackPosItem(0),
        myRouteItem(0),
        myPhotoItem(0),
        myShowGrid(true),
        myShowTileBounds(false),
        myShowTrackBb(false)
{
    connect(myModel, SIGNAL(mapChanged()), this, SLOT(redraw()));
    connect(myModel, SIGNAL(trackChanged()), this, SLOT(redrawTrack()));
    connect(myModel, SIGNAL(trackPosChanged(int)), this, SLOT(changeTrackPos(int)));
    connect(myModel, SIGNAL(routeChanged()), this, SLOT(redrawRoute()));
    connect(myModel, SIGNAL(routePointMoved(int)), this, SLOT(changeRoutePos(int)));
    connect(myModel, SIGNAL(waypointsChanged()), this, SLOT(redrawWaypoints()));
    connect(&getter, SIGNAL(done(bool)), this, SLOT(tileLoaded(bool)));
}

void MapScene::redrawLayer(const Layer& layer, int z, const QMap<QString, QPixmap> &oldPixmaps) {
    int x = myModel->x();
    int y = myModel->y();
    int w = myModel->width();
    int h = myModel->height();
    int pow2 = 1<<myModel->zoom();
    for (int ix = 0; ix < w; ix++) {
        for (int iy = 0; iy < h; iy++) {
            int cx = x+ix;
            int cy = y+iy;
            if (cx < 0 || cy < 0 || cx >= pow2 || cy >= pow2)
                continue;
            QString key = layer.url();
            key = key.replace(QString("$z"), QString::number(myModel->zoom()))
                     .replace(QString("$y"), QString::number(cy))
                     .replace(QString("$x"), QString::number(cx));
            if (oldPixmaps.contains(key)) {
                addPixmap(key, oldPixmaps.value(key), ix*256, iy*256, z);
                continue;
            }
            QPixmap px = myModel->getPixmap(key);
            if (!px.isNull()) {
                addPixmap(key, px, ix*256, iy*256, z);
                continue;
            }
            requests.append(TileRequest(key, QPoint(ix*256, iy*256), z));
        }
    }
}

void MapScene::redraw() {
    qDebug()<<"redraw "<<myModel->layer().name();
    qDeleteAll(items());
    gridGroup.clear();
    trackGroup.clear();
    tileBoundGroup.clear();
    myRoutePointItems.clear();
    myTrackPointItems.clear();
    myWaypointItems.clear();
    QMap<QString, QPixmap> oldPixmaps = myPixmaps;
    myPixmaps.clear();
    myTrackItem = NULL;
    myTrackPosItem = NULL;
    myRouteItem = NULL;
    myPhotoItem = 0;
    int w = myModel->width();
    int h = myModel->height();
    progressDlg = new QProgressDialog(tr("Download tiles"), tr("&Cancel"), 0, w*h);
    connect(progressDlg, SIGNAL(canceled()), this, SLOT(cancelRequests()));
    connect(progressDlg, SIGNAL(destroyed()), this, SLOT(cancelRequests()));
    progressDlg->setModal(true);
    progressDlg->show();
    redrawLayer(myModel->layer(), 0, oldPixmaps);
    QList<Layer> *ovls = myModel->overlays();
    for (int i = 0; i < ovls->size(); i++) {
        Layer ovl = ovls->at(i);
        redrawLayer(ovl, i+1, oldPixmaps);
    }
    setSceneRect(0, 0, w*256, h*256);
    progressDlg->setMaximum(requests.size());
    getNextTile();
}

QPixmap MapScene::getPixmap(const QString &key) const {
    if (myPixmaps.contains(key)) return myPixmaps.value(key);
    return QPixmap();
}

void MapScene::addPixmap(const QString&key, const QPixmap& pixmap, int ix, int iy, int z) {
    if (pixmap.isNull()) return;
    QGraphicsPixmapItem *it = new QGraphicsPixmapItem(pixmap);
    it->setOffset(ix, iy);
    it->setZValue(z);
    addItem(it);
    myPixmaps.insert(key, pixmap);
}

void MapScene::getNextTile() {
    while(requests.size() > 0) {
        TileRequest req = requests[0];
        QString url = req.key();
        qDebug()<<url;
        if (getter.getFile(QUrl(url), req, progressDlg)) {
            return;
        }
        else {
            requests.removeAt(0);
        }
    }
//    qDebug()<<"Cache entries:";
//    foreach(PixmapEntry pe, myModel->pixmaps()) {
//        qDebug()<<pe.key();
//    }

    progressDlg->hide();
    delete progressDlg;
    progressDlg = NULL;
    redrawTrack();
    redrawGrid();
    redrawTileBounds();
    redrawRoute();
    redrawWaypoints();
    redrawPhoto();
    update();
}

void MapScene::tileLoaded(bool error) {
    TileRequest req = getter.request();
    qDebug()<<"tileLoaded "<<req.pos()<<error;
    if (requests.contains(req)) {
        requests.removeOne(req);
        if (!error) {
            QPoint p = req.pos();
            QPixmap pixmap;
            qDebug()<<"pixmap";
            //getter->close();
            bool res = pixmap.loadFromData(getter.getData(), 0, Qt::AutoColor|Qt::NoOpaqueDetection);
            qDebug()<<"...loaded"<<res;
            if (res) {
                myModel->savePixmap(req.key(), pixmap);
                addPixmap(req.key(), pixmap, p.x(), p.y(), req.z());
            }
        }
    }
    //getter.close();
    qDebug()<<"Request queue "<<requests.size();
    progressDlg->setValue(progressDlg->maximum()-requests.size());
    getNextTile();
    //progressDlg->setValue(getters.size());
}

void MapScene::cancelRequests() {
    if (requests.size() == 0)
        return;
    qDebug()<<"cancelRequests";
    requests.clear();
    getter.abort();
}

void MapScene::redrawTrack() {
    qDeleteAll(myTrackPointItems);
    myTrackPointItems.clear();
    foreach(QGraphicsItem *it, trackGroup) {
        delete it;
    }
    trackGroup.clear();
    myTrackItem = NULL;
    myTrackPosItem = NULL;
    const Track& track = myModel->track();
    if (track.isEmpty()) return;
    QPolygonF points;
    foreach (const GpxPoint& p, track.trackPoints()) {
        QPointF pt = myModel->lonLat2Scene(p.coord());
        points.append(pt);
        TrackPointItem *it = new TrackPointItem(pt, p.sym());
        myTrackPointItems.append(it);
        addItem(it);
    }
    /* adjust scene rect
    QRectF pbb = points.boundingRect();
    QRectF sbb = sceneRect();
    if (sbb.x() > pbb.x()) sbb.setX(pbb.x());
    if (sbb.y() > pbb.y()) sbb.setY(pbb.y());
    if (sbb.x()+sbb.width() < pbb.x()+pbb.width()) sbb.setWidth(pbb.x()+pbb.width()-sbb.x());
    if (sbb.y()+sbb.height() < pbb.y()+pbb.height()) sbb.setHeight(pbb.y()+pbb.height()-sbb.y());
    setSceneRect(sbb);
    */
    //qDebug()<<"redrawTrack "<<points;
    if (myTrackItem == NULL) {
        myTrackItem = new TrackItem(points);
        addItem(myTrackItem);
        trackGroup.append(myTrackItem);
    }
    else {
        myTrackItem->setPoints(points);
    }
    /*
    foreach(Waypoint wpt, track->waypoints()) {
        QPointF pt = myModel->lonLat2Scene(wpt.lonLat());
        WaypointItem *it = new WaypointItem(wpt.name());
        addItem(it);
        trackGroup.append(it);
        it->setPos(pt);
    }
    */
    if (myShowTrackBb) {
        BoundingBox bb = track.boundingBox();
        qDebug()<<"bb "<<bb.p0()<<" "<<bb.p1();
        QPointF p0 = myModel->lonLat2Scene(bb.p0());
        QPointF p1 = myModel->lonLat2Scene(bb.p1());
        qDebug()<<"bb: "<<p0<<", "<<p1;
        QGraphicsRectItem *it = new QGraphicsRectItem(p0.x()-10, p1.y()-10, p1.x()-p0.x()+20, p0.y()-p1.y()+20);
        it->setPen(QPen(QBrush(Qt::magenta), 0));
        addItem(it);
        trackGroup.append(it);
    }
    changeTrackPos(track.pos());
}

void MapScene::setShowGrid(bool show) {
    myShowGrid = show;
    redrawGrid();
}

void MapScene::setShowTileBounds(bool show) {
    myShowTileBounds = show;
    redrawTileBounds();
}

void MapScene::setShowTrackBb(bool show) {
    myShowTrackBb = show;
    redrawTrack();
}

void MapScene::redrawGrid() {
    qDebug()<<"redrawGrid "<<myShowGrid;

    foreach(QGraphicsItem *it, gridGroup) {
        removeItem(it);
        delete it;
    }
    gridGroup.clear();
    if (!myShowGrid) {
        return;
    }
    QPointF lonLat0 = myModel->lonLat(QPointF(0, 0));
    QPointF lonLat1 = myModel->lonLat(QPointF(0, myModel->height()*256));
    double distKm = Model::geodist1(lonLat0, lonLat1);
    double lineDist = myModel->height()*256/distKm;
    while (lineDist < 20)
        lineDist *= 10;
    int w = myModel->width()*256;
    int h = myModel->height()*256;
    for (double dx = lineDist; dx < w; dx += lineDist) {
        QGraphicsLineItem *it = new QGraphicsLineItem(dx, 0, dx, h);
        it->setPen(QPen(qRgba(127, 127, 127, 128)));
        it->setZValue(100);
        addItem(it);
        gridGroup.append(it);
    }
    for (double dy = lineDist; dy < h; dy += lineDist) {
        QGraphicsLineItem *it = new QGraphicsLineItem(0, dy, w, dy);
        it->setPen(QPen(qRgba(127, 127, 127, 128)));
        it->setZValue(100);
        addItem(it);
        gridGroup.append(it);
    }
}

void MapScene::redrawTileBounds() {
    foreach(QGraphicsItem *it, tileBoundGroup) {
        removeItem(it);
        delete it;
    }
    tileBoundGroup.clear();
    if (!myShowTileBounds)
        return;
    int w = model()->width();
    int h = model()->height();
    for (int i = 1; i < w; i++) {
        QGraphicsLineItem *it = new QGraphicsLineItem(i*256, 0, i*256, h*256);
        addItem(it);
        tileBoundGroup.append(it);
    }
    for (int i = 1; i < h; i++) {
        QGraphicsLineItem *it = new QGraphicsLineItem(0, i*256, w*256, i*256);
        addItem(it);
        tileBoundGroup.append(it);
    }
}

void MapScene::changeTrackPos(int pos) {
    if (myModel->track().isEmpty()) return;
    if (pos < 0) pos = 0;
    if (pos >= myModel->track().trackPoints().size()) pos = myModel->track().trackPoints().size()-1;
    GpxPoint p = myModel->track().trackPoint(pos);
    QPointF pt = myModel->lonLat2Scene(p.coord());
    if (myTrackPosItem == 0) {
        myTrackPosItem = new TrackPosItem(pt);
        myTrackPosItem->setZValue(10);
        addItem(myTrackPosItem);
        trackGroup.append(myTrackPosItem);
    } else {
        QRectF oldRect = myTrackPosItem->sceneBoundingRect();
        myTrackPosItem->setPos(pt);
        invalidate(oldRect);
    }
}

void MapScene::hideTrack(bool showTrackLine) {
    foreach(QGraphicsItem *it, trackGroup) {
        it->setVisible(false);
    }
    if (showTrackLine && myTrackItem != NULL) {
        myTrackItem->setVisible(true);
    }
}

void MapScene::showTrack() {
    foreach(QGraphicsItem *it, trackGroup) {
        it->setVisible(true);
    }
}

void MapScene::showPhotoItem(const QPointF &pos) {
    myPhotoCoord.clear();
    myPhotoCoord.push_back(pos);
    redrawPhoto();
}

void MapScene::hidePhotoItem() {
    myPhotoCoord.clear();
    redrawPhoto();
}

void MapScene::redrawRoute() {
    QPolygonF points;
    qDeleteAll(myRoutePointItems);
    myRoutePointItems.clear();
    foreach (const GpxPoint& point, *myModel->route().points()) {
        QPointF pt = myModel->lonLat2Scene(point.coord());
        points.append(pt);
        RoutePointItem *it = new RoutePointItem(pt, point.sym());
        addItem(it);
        myRoutePointItems.append(it);
    }
    if (points.size()<2) {
        if (myRouteItem != NULL)
            removeItem(myRouteItem);
        delete myRouteItem;
        myRouteItem = 0;
    }
    else {
        if (myRouteItem == NULL) {
            myRouteItem = new RouteItem(points);
            addItem(myRouteItem);
        }
        else {
            myRouteItem->setPoints(points);
        }
    }
}

void MapScene::changeRoutePos(int idx) {
    GpxPoint routePoint = (*myModel->route().points())[idx];
    QPointF p = myModel->lonLat2Scene(routePoint.coord());
    myRoutePointItems[idx]->setPoint(p);
    myRoutePointItems[idx]->setSym(routePoint.sym());
    QPolygonF points;
    foreach (const GpxPoint& point, *myModel->route().points()) {
        QPointF pt = myModel->lonLat2Scene(point.coord());
        points.append(pt);
    }
    if (points.size() >= 2) {
        myRouteItem->setPoints(points);
    }
}

void MapScene::redrawWaypoints() {
    qDebug()<<"redrawWaypoints";
    foreach(QGraphicsItem *it, myWaypointItems) {
        delete it;
    }
    myWaypointItems.clear();
    foreach (const GpxPoint& p, myModel->waypoints()) {
        qDebug()<<"waypoint "<<p.name()<<" "<<p.coord();
        WaypointItem *it = new WaypointItem(p.name());
        QPointF coord = myModel->lonLat2Scene(p.coord());
        it->setPos(coord);
        addItem(it);
        myWaypointItems.append(it);
    }
}

void MapScene::redrawPhoto() {
    delete myPhotoItem;
    myPhotoItem = 0;
    if (myPhotoCoord.size() == 0) return;
    QPointF pt = myModel->lonLat2Scene(myPhotoCoord[0]);
    myPhotoItem = new PhotoItem(pt);
    addItem(myPhotoItem);
}
