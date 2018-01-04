#include <QtWidgets>
#include <QtDebug>
#include "deltrkpartdlg.h"
#include "geom.h"
#include "model.h"
#include "mapscene.h"
#include "mapview.h"
#include "route.h"
#include "gpxpointdlg.h"
#include "track.h"
#include "viewfunction.h"

MapView::MapView(QGraphicsScene *scene, Settings *settings) :
    QGraphicsView(scene), mySettings(settings),
    tempItem(NULL)
{
    function.reset(new ShowFunction(this));
    setMouseTracking(true);
    createActions();
}

void MapView::createActions() {
    selectTrackPointAction = new QAction(tr("Select Track Point"), this);
    insertTrackPointAction = new QAction(tr("Insert Track Point"), this);
    editTrackPointAction = new QAction(tr("Edit Track Point"), this);
    delTrackPointAction = new QAction(tr("Delete Track Point"), this);
    editRoutePointAction = new QAction(tr("Edit Route Point"), this);
    insertRoutePointAction = new QAction(tr("Insert Route Point"), this);
    delRoutePointAction = new QAction(tr("Delete Route Point"), this);
    editWaypointAction = new QAction(tr("Edit Waypoint"), this);
    delWaypointAction = new QAction(tr("Delete Waypoint"), this);
}

void MapView::mousePressEvent(QMouseEvent *event) {
    QPoint vpos = event->pos();
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    QGraphicsItem *currIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<it<<" "<<it->type();
        if (it->type() == RoutePointItem::Type) {
            currIt = it;
            break;
        }
        if (it->type() == TrackPointItem::Type) {
            currIt = it;
            break;
        }
        if (it->type() == WaypointItem::Type) {
            currIt = it;
            break;
        }
    }
    QPointF pos = mapToScene(event->pos());
    if (event->button() == Qt::LeftButton) {
        function->b1(pos, currIt);
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
    myPos = mapToScene(event->pos());
    QPointF lonLat = model->lonLat(myPos);
    //qDebug()<<"mouse "<<event->pos()<<" scrollbars: "<<horizontalScrollBar()->value()<<","<<verticalScrollBar()->value()
    //       <<" size: "<<width()<<", "<<height();
    //qDebug()<<"lonLat: "<<lonLat;
    if ((event->buttons() & Qt::RightButton) != 0) {
        qDebug()<<"motion "<<myPos.x()-mouseX0<<" "<<myPos.y()-mouseY0;
        QScrollBar *sb = verticalScrollBar();
        qDebug()<<"vsb "<<sb->value();
        sb->setValue(sb->value()-myPos.y()+mouseY0);
        sb = horizontalScrollBar();
        qDebug()<<"hsb "<<sb->value();
        sb->setValue(sb->value()-myPos.x()+mouseX0);
    }
    else if (!event->buttons()) {
        function->motion(myPos);
    }
    emit mouseGeoPos(lonLat);
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::keyPressEvent(QKeyEvent *event) {
    if (function->key(event)) return;
    if (event->matches(QKeySequence::ZoomIn)) {
        zoomIn(myPos);
        return;
    }
    if (event->matches(QKeySequence::ZoomOut)) {
        zoomOut(myPos);
        return;
    }
    QGraphicsView::keyPressEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() != Qt::ControlModifier) {
        QGraphicsView::wheelEvent(event);
        return;
    }
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

void MapView::contextMenuEvent(QContextMenuEvent *event) {
    QPoint vpos = event->pos();
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    qDebug()<<"contextMenu"<<vpos<<" bb "<<bb;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<it<<" "<<it->type();
        if (it->type() == RoutePointItem::Type) {
            qDebug()<<"routePointItem";
            QList<QAction*> actions;
            actions<<editRoutePointAction<<delRoutePointAction;
            QAction *action = QMenu::exec(actions, mapToGlobal(vpos), 0, this);
            if (action == editRoutePointAction) {
                editRoutePoint(mapToScene(vpos));
            } else if (action == delRoutePointAction) {
                delRoutePoint(mapToScene(vpos));
            }
            break;
        } else if (it->type() == RouteItem::Type) {
            QList<QAction*> actions;
            actions<<insertRoutePointAction;
            QAction *action = QMenu::exec(actions, mapToGlobal(vpos), 0, this);
            if (action == insertRoutePointAction) {
                insertRoutePoint(mapToScene(vpos));
            }
            break;
        } else if (it->type() == TrackPointItem::Type) {
            QList<QAction*> actions;
            actions<<selectTrackPointAction<<editTrackPointAction<<delTrackPointAction;
            QAction *action = QMenu::exec(actions, mapToGlobal(vpos), 0, this);
            if (action == selectTrackPointAction) {
                selectTrackPoint(mapToScene(vpos));
            }
            else if (action == editTrackPointAction) {
                editTrackPoint(mapToScene(vpos));
            } else if (action == delTrackPointAction) {
                delTrackPoint(mapToScene(vpos));
            }
            break;
        } else if (it->type() == TrackItem::Type) {
            qDebug()<<"TrackItem";
            QList<QAction*> actions;
            actions<<insertTrackPointAction;
            QAction *action = QMenu::exec(actions, mapToGlobal(vpos), 0, this);
            if (action == insertTrackPointAction) {
                insertTrackPoint(mapToScene(vpos));
            }
            break;
        } else if (it->type() == WaypointItem::Type) {
            QList<QAction*> actions;
            actions<<editWaypointAction<<delWaypointAction;
            QAction *action = QMenu::exec(actions, mapToGlobal(vpos), 0, this);
            if (action == editWaypointAction) {
                editWaypoint(mapToScene(vpos));
            } else if (action == delWaypointAction) {
                if (QMessageBox::question(this, tr("Delete Waypoint"), tr("Delete Waypoint?"),
                                          QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                        == QMessageBox::Yes) {
                    delWaypoint(mapToScene(vpos));
                }
            }
            break;
        }
    }
}

void MapView::centerView() {
    QScrollBar *hsb = horizontalScrollBar();
    QScrollBar *vsb = verticalScrollBar();
    hsb->setValue(hsb->maximum()/2);
    vsb->setValue(vsb->maximum()/2);
}

void MapView::zoomIn(const QPointF& pos) {
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF lonLat = model->lonLat(pos);
    model->zoomIn(lonLat);
    centerView();
}

void MapView::zoomOut(const QPointF& pos) {
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF lonLat = model->lonLat(pos);
    model->zoomOut(lonLat);
    centerView();
}

void MapView::zoomInCenter() {
    QPoint vCenter(width()/2, height()/2);
    qDebug()<<"zoomIn center "<<vCenter;
    zoomIn(mapToScene(vCenter));
}

void MapView::zoomOutCenter() {
    QPoint vCenter(width()/2, height()/2);
    qDebug()<<"zoomOut center "<<vCenter;
    zoomOut(mapToScene(vCenter));
}

void MapView::showPos(const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    if (mapScene->trackItem() != NULL) {
        QList<QGraphicsItem*> its = mapScene->items(QRect(pos.x()-3, pos.y()-3, 7, 7));
        if (its.contains(mapScene->trackItem())) {
            int idx = model->track().nearest(lonLat);
            model->setTrackPos(idx);
        }
    }
}

int MapView::idxOfTrackPoint(const QPointF& pos) {
    QPointF vpos = mapFromScene(pos);
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    TrackPointItem *tIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<"Found "<<it<<" "<<it->type()<<" "<<RoutePointItem::Type;
        if (it->type() == TrackPointItem::Type) {
            tIt = qgraphicsitem_cast<TrackPointItem *>(it);
            qDebug()<<tIt;
            //break;
        }
    }
    if (!tIt) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->trackPointItems().indexOf(tIt);
}

int MapView::idxOfTrackPoint(TrackPointItem *it) {
    if (!it) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->trackPointItems().indexOf(it);
}

void MapView::moveTrackPoint(int idx, const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    // don't emit track change
    model->trackPtr()->setTrackPointPos(idx, lonLat);
    mapScene->redrawTrack();
}

void MapView::editTrackPoint(const QPointF& pos) {
    int idx = idxOfTrackPoint(pos);
    if (idx < 0) return;
    editTrackPoint(idx);
}

void MapView::editTrackPoint(int idx) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    const GpxPointList& points = model->track().trackPoints();
    double dist0 = Model::geodist1(points, 0, idx);
    double dist1 = Model::geodist1(points, idx, points.size()-1);
    GpxPointDlg dlg(model, model->track().trackPoint(idx), mySettings->mapIconList(), true);
    dlg.setSrtmEle(model->srtmEle(points[idx].coord()));
    dlg.setDists(dist0, dist1);
    createTempPoint(points[idx].coord());
    connect(&dlg, SIGNAL(posChanged(const QPointF&)), this, SLOT(moveTempPoint(QPointF)));
    if (dlg.exec()) {
        model->setTrackPoint(idx, dlg.point());
    }
    deleteTempPoint();
}

void MapView::selectTrackPoint(const QPointF& pos) {
    int idx = idxOfTrackPoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    mapScene->model()->setTrackPos(idx);
}

void MapView::delTrackPoint(const QPointF& pos) {
    int idx = idxOfTrackPoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    const GpxPoint& p = model->track().trackPoint(idx);
    if (QMessageBox::question(this, tr("Delete Trackpoint"),
                              tr("Delete trackpoint %1 %2 (%3)?").arg(idx).arg(p.name()).arg(p.sym()),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            != QMessageBox::Yes) {
        return;
    }
    model->delTrackPoint(idx);
}

void MapView::insertTrackPoint(const QPointF &pos) {
    QPointF vpos = mapFromScene(pos);
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    TrackItem *tIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<"Found "<<it<<" "<<it->type()<<" "<<TrackItem::Type;
        if (it->type() == TrackItem::Type) {
            tIt = qgraphicsitem_cast<TrackItem *>(it);
            qDebug()<<tIt;
            break;
        }
    }
    if (!tIt) return;
    qDebug()<<"track found "<<pos;
    qDebug()<<tIt->points();
    IdxPointF ipt = insertPoint(tIt->points(), pos);
    if (ipt.idx() > 0) {
        MapScene *mapScene = static_cast<MapScene*>(scene());
        Model *model = mapScene->model();
        GpxPoint p0 = model->track().trackPoint(ipt.idx()-1);
        GpxPoint p1 = model->track().trackPoint(ipt.idx());
        QPointF lonLat = model->lonLat(ipt.point());
        double t = ipt.t();
        double ti0 = p0.timestamp().toTime_t();
        double ti1 = p1.timestamp().toTime_t();
        int ti = int(round((1.0-t)*ti0+t*ti1));
        GpxPoint p(GpxPoint::TRK, lonLat, QDateTime::fromTime_t(ti), (1.0-t)*p0.ele()+t*p1.ele());
        model->insertTrackPoint(ipt.idx(), p);
    }
}


void MapView::newRoutePoint(const QPointF &pos) {
    qDebug()<<"newRoutePoint "<<pos;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    int ele = model->srtmEle(lonLat);
    Route *route = model->routePtr();
    GpxPoint pt(GpxPoint::RTE, lonLat, QDateTime(), ele);
    pt.setSrtm(ele);
    route->newRoutePoint(pt);
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
    return mapScene->routePointItems().indexOf(rIt);
}

int MapView::idxOfRoutePoint(RoutePointItem *it) {
    if (!it) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->routePointItems().indexOf(it);
}

void MapView::delRoutePoint(const QPointF& pos) {
    int idx = idxOfRoutePoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    const GpxPoint& p = model->route().points()->at(idx);
    if (QMessageBox::question(this, tr("Delete Route Point"),
                              tr("Delete route point %1, %2 (%3)?").arg(idx).arg(p.name()).arg(p.sym()),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            != QMessageBox::Yes) {
        return;
    }
    model->routePtr()->delRoutePoint(idx);
}

void MapView::moveRoutePoint(int idx, const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    mapScene->model()->changeRoutePoint(idx, lonLat);
}

void MapView::editRoutePoint(const QPointF& pos) {
    int idx = idxOfRoutePoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    const GpxPointList *points = model->route().points();
    GpxPointDlg dlg(model, points->at(idx), mySettings->mapIconList(), true);
    double dist0 = Model::geodist1(*points, 0, idx);
    double dist1 = Model::geodist1(*points, idx, points->size()-1);
    //Test
    //double dist1 = Model::geodist0(*points, 0, idx);
    dlg.setSrtmEle(model->srtmEle(points->at(idx).coord()));
    dlg.setDists(dist0, dist1);
    createTempPoint(points->at(idx).coord());
    connect(&dlg, SIGNAL(posChanged(const QPointF&)), this, SLOT(moveTempPoint(QPointF)));
    if (dlg.exec()) {
        model->setRoutePoint(idx, dlg.point());
    }
    deleteTempPoint();
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
        int ele = model->srtmEle(lonLat);
        model->routePtr()->insertRoutePoint(ipt.idx(), GpxPoint(GpxPoint::RTE, lonLat, QDateTime(), ele));
    }
}

int MapView::idxOfWaypoint(const QPointF& pos) {
    QPointF vpos = mapFromScene(pos);
    QRect bb(vpos.x()-3, vpos.y()-3, 7, 7);
    WaypointItem *wIt = 0;
    foreach(QGraphicsItem *it, items(bb)) {
        qDebug()<<"Found "<<it<<" "<<it->type()<<" "<<RoutePointItem::Type;
        if (it->type() == WaypointItem::Type) {
            wIt = qgraphicsitem_cast<WaypointItem *>(it);
            break;
        }
    }
    if (!wIt) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->waypointItems().indexOf(wIt);
}

int MapView::idxOfWaypoint(WaypointItem *it) {
    if (!it) return -1;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    return mapScene->waypointItems().indexOf(it);
}

void MapView::newWaypoint(const QPointF &pos, const QString& name) {
    qDebug()<<"newWaypoint "<<pos<<" "<<name;
    if (name.trimmed().isNull()) return;
    Model *model = static_cast<MapScene*>(scene())->model();
    QPointF gpos = model->lonLat(pos);
    int ele = model->srtmEle(gpos);
    model->addWaypoint(GpxPoint(GpxPoint::WPT, gpos, QDateTime(), ele, "flag", name.trimmed()));
}

void MapView::moveWaypoint(int idx, const QPointF& pos) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    QPointF lonLat = model->lonLat(pos);
    mapScene->model()->changeWaypoint(idx, lonLat);
}

void MapView::editWaypoint(const QPointF& pos) {
    int idx = idxOfWaypoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    GpxPointDlg dlg(model, model->waypoints().at(idx), mySettings->mapIconList(), true);
    //Test
    //double dist1 = Model::geodist0(*points, 0, idx);
    dlg.setSrtmEle(model->srtmEle(model->waypoints().at(idx).coord()));
    createTempPoint(model->waypoints().at(idx).coord());
    connect(&dlg, SIGNAL(posChanged(const QPointF&)), this, SLOT(moveTempPoint(QPointF)));
    if (dlg.exec()) {
        model->setWaypoint(idx, dlg.point());
    }
    deleteTempPoint();
}

void MapView::delWaypoint(const QPointF& pos) {
    int idx = idxOfWaypoint(pos);
    if (idx < 0) return;
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = mapScene->model();
    model->delWaypoint(idx);
}

void MapView::setShowFunction() {
    function.reset(new ShowFunction(this));
}

void MapView::setMoveTrackPosFunction() {
    function.reset(new SetTrackPosFunction(this));
}

void MapView::setMoveGpxPointFunction() {
    function.reset(new MoveGpxPointFunction(this));
}

void MapView::setNewRoutePointFunction() {
    function.reset(new NewRoutePointFunction(this));
}

void MapView::setDelRoutePointFunction() {
    function.reset(new DelRoutePointFunction(this));
}

void MapView::setMoveRoutePointFunction() {
    function.reset(new MoveRoutePointFunction(this));
}

void MapView::setEditRoutePointFunction() {
    function.reset(new EditRoutePointFunction(this));
}

void MapView::setInsertRoutePointFunction() {
    function.reset(new InsertRoutePointFunction(this));
}

void MapView::setNewWaypointFunction() {
    function.reset(new NewWaypointFunction(this));
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

void MapView::delTrackPart() {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    Model *model = static_cast<MapScene*>(scene())->model();
    DelTrkPartDlg dlg(mapScene);
    if (dlg.exec() == QDialog::Accepted) {
        int i0 = dlg.pos0();
        int i1 = dlg.pos1();
        model->delTrackPart(i0, i1);
    }
}

void MapView::changeTrackPos(int) {
    MapScene *mapScene = static_cast<MapScene*>(scene());
    ensureVisible(mapScene->trackPosItem());
}
