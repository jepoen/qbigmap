#include <QtGui>
#include <QtDebug>
#include "geom.h"
#include "model.h"
#include "mapscene.h"
#include "mapview.h"
#include "route.h"
#include "routepointdlg.h"
#include "track.h"
#include "viewfunction.h"

MapView::MapView(QGraphicsScene *scene, Settings *settings) :
    QGraphicsView(scene), mySettings(settings),
    tempItem(NULL)
{
    function = new ShowFunction(this);
    setMouseTracking(true);
}

void MapView::mousePressEvent(QMouseEvent *event) {
    QPointF pos = mapToScene(event->pos());
    if (event->button() == Qt::LeftButton) {
        function->b1(pos);
    }
    else if (event->button() == Qt::MidButton) {
        function->b2(pos);
    }
    else if (event->button() == Qt::RightButton) {
        mouseX0 = pos.x();
        mouseY0 = pos.y();
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF pos = mapToScene(event->pos());
    QPointF lonLat = model->lonLat(pos);
    //qDebug()<<"lonLat: "<<lonLat;
    if ((event->buttons() & Qt::RightButton) != 0) {
        qDebug()<<"motion "<<pos.x()-mouseX0<<" "<<pos.y()-mouseY0;
        QScrollBar *sb = verticalScrollBar();
        qDebug()<<"vsb "<<sb->value();
        sb->setValue(sb->value()-pos.y()+mouseY0);
        sb = horizontalScrollBar();
        qDebug()<<"hsb "<<sb->value();
        sb->setValue(sb->value()-pos.x()+mouseX0);
    }
    else if (!event->buttons()) {
        function->motion(pos);
    }
    emit mouseGeoPos(lonLat);
}

void MapView::wheelEvent(QWheelEvent *event) {
    QPointF pos = mapToScene(event->pos());
    int ix = (int)pos.x()/256;
    int iy = (int)pos.y()/256;
    qDebug()<<"Mousepos: "<<ix<<" "<<iy<<" "<<event->delta();
    if (event->delta() > 0) {
        zoomIn(pos);
    }
    else if (event->delta() < 0) {
        zoomOut(pos);
    }
}

void MapView::zoomIn(const QPointF& pos) {
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF lonLat = model->lonLat(pos);
    model->zoomIn(lonLat);
}

void MapView::zoomOut(const QPointF& pos) {
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF lonLat = model->lonLat(pos);
    model->zoomOut(lonLat);
}

void MapView::showPos(const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    if (mapScene->trackItem() != NULL) {
        QList<QGraphicsItem*> its = mapScene->items(QRect(pos.x()-3, pos.y()-3, 7, 7));
        if (its.contains(mapScene->trackItem())) {
            int idx = model->track()->nearest(lonLat);
            model->setTrackPos(idx);
        }
    }

}

void MapView::newRoutePoint(const QPointF &pos) {
    qDebug()<<"newRoutePoint "<<pos;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    Route *route = model->route();
    route->newRoutePoint(RoutePoint(lonLat));
}

int MapView::idxOfRoutePoint(const QPointF& pos) {
    QPointF vpos = mapFromScene(pos);
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    RoutePointItem *rIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<"Found "<<it<<" "<<it->type()<<" "<<RoutePointItem::Type;
        if (it->type() == RoutePointItem::Type) {
            rIt = qgraphicsitem_cast<RoutePointItem *>(it);
            qDebug()<<rIt;
            //break;
        }
    }
    if (!rIt) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->RoutePointItems().indexOf(rIt);
}

void MapView::delRoutePoint(const QPointF& pos) {
    int idx = idxOfRoutePoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    model->route()->delRoutePoint(idx);
}

void MapView::moveRoutePoint(int idx, const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    mapScene->model()->route()->moveRoutePoint(idx, lonLat);
}

void MapView::editRoutePoint(const QPointF& pos) {
    int idx = idxOfRoutePoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    RoutePointDlg dlg(model->route()->points()->at(idx), mySettings->mapIcons());
    if (dlg.exec()) {
        model->route()->updateRoutePoint(idx, dlg.point());
    }
}

void MapView::insertRoutePoint(const QPointF &pos) {
    QPointF vpos = mapFromScene(pos);
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    RouteItem *rIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<"Found "<<it<<" "<<it->type()<<" "<<RouteItem::Type;
        if (it->type() == RouteItem::Type) {
            rIt = qgraphicsitem_cast<RouteItem *>(it);
            qDebug()<<rIt;
            break;
        }
    }
    if (!rIt) return;
    qDebug()<<"route found "<<pos;
    qDebug()<<rIt->points();
    IdxPointF ipt = insertPoint(rIt->points(), pos);
    if (ipt.idx() > 0) {
        MapScene *mapScene = static_cast<MapScene*>(scene());
        Model *model = mapScene->model();
        QPointF lonLat = model->lonLat(ipt.point());
        model->route()->insertRoutePoint(ipt.idx(), RoutePoint(lonLat));
    }
}

void MapView::setShowFunction() {
    delete function;
    function = new ShowFunction(this);
}

void MapView::setZoomInFunction() {
    delete function;
    function = new ZoomInFunction(this);
}

void MapView::setZoomOutFunction() {
    delete function;
    function = new ZoomOutFunction(this);
}

void MapView::setNewRoutePointFunction() {
    delete function;
    function = new NewRoutePointFunction(this);
}

void MapView::setDelRoutePointFunction() {
    delete function;
    function = new DelRoutePointFunction(this);
}

void MapView::setMoveRoutePointFunction() {
    delete function;
    function = new MoveRoutePointFunction(this);
}

void MapView::setEditRoutePointFunction() {
    delete function;
    function = new EditRoutePointFunction(this);
}

void MapView::setInsertRoutePointFunction() {
    delete function;
    function = new InsertRoutePointFunction(this);
}

void MapView::createTempPoint(const QPointF& pos) {
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF gpos = model->lonLat2Scene(pos);
    tempItem = new QGraphicsEllipseItem(-3, -3, 6, 6);
    scene()->addItem(tempItem);
    tempItem->setPos(gpos);
}

void MapView::moveTempPoint(const QPointF& pos) {
    if (tempItem == NULL)
        return;
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF gpos = model->lonLat2Scene(pos);
    tempItem->setPos(gpos);
}

void MapView::deleteTempPoint() {
    if (tempItem == NULL)
        return;
    delete tempItem;
    tempItem = NULL;
}
