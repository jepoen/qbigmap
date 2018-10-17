#include <QtGui>
#include <QtDebug>
#include "mapscene.h"
#include "mapview.h"
#include "route.h"
#include "viewfunction.h"

ViewFunction::ViewFunction(MapView *view) :
    myView(view)
{}

ShowFunction::ShowFunction(MapView *view) :
        ViewFunction(view)
{}

void ShowFunction::b1(const QPointF& pos, QGraphicsItem */*it*/) {
    myView->showPos(pos);
}

SetTrackPosFunction::SetTrackPosFunction(MapView *view) :
    ViewFunction(view)
{}

void SetTrackPosFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    int idx = myView->idxOfTrackPoint(pos);
    MapScene *scene = static_cast<MapScene*>(myView->scene());
    scene->model()->setTrackPos(idx);
}

MoveGpxPointFunction::MoveGpxPointFunction(MapView *view) :
    ViewFunction(view),
    myState(0), myIdx(-1), myType(0)
{}

void MoveGpxPointFunction::b1(const QPointF &pos, QGraphicsItem *it) {
    if (myState == 0) {
        if (it == 0) return;
        qDebug()<<"move gpx point "<<pos<<" it "<<it<<it->type();
        myType = it->type();
    }
    switch (myType) {
    case TrackPointItem::Type:
        b1_trackpoint(pos);
        break;
    case RoutePointItem::Type:
        b1_routepoint(pos);
        break;
    case WaypointItem::Type:
        b1_waypoint(pos);
    }
}

void MoveGpxPointFunction::b1_trackpoint(const QPointF& pos) {
    if (myState == 0) {
        myIdx = myView->idxOfTrackPoint(pos);
        if (myIdx >= 0) {
            MapScene *scene = static_cast<MapScene*>(myView->scene());
            myOldPos = scene->model()->track().trackPoint(myIdx).coord();
            myState = 1;
        }
        qDebug()<<" move track point "<<myIdx;
    }
    else {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        QPointF lonLat = scene->model()->lonLat(pos);
        scene->model()->changeTrackPoint(myIdx, lonLat);
        myState = 0;
        myIdx = -1;
        myType = 0;
    }
}

void MoveGpxPointFunction::b1_routepoint(const QPointF &pos) {
    if (myState == 0) {
        myIdx = myView->idxOfRoutePoint(pos);
        if (myIdx >= 0) {
            MapScene *scene = static_cast<MapScene*>(myView->scene());
            myOldPos = scene->model()->route().points()->at(myIdx).coord();
            myState = 1;
        }
    }
    else {
        myState = 0;
        myIdx = -1;
        myType = 0;
    }
}

void MoveGpxPointFunction::b1_waypoint(const QPointF &pos) {
    if (myState == 0) {
        qDebug()<<"b1_waypoint";
        myIdx = myView->idxOfWaypoint(pos);
        if (myIdx >= 0) {
            qDebug()<<" waypoint "<<myIdx;
            MapScene *scene = static_cast<MapScene*>(myView->scene());
            myOldPos = scene->model()->waypoints().at(myIdx).coord();
            myState = 1;
        }
    }
    else {
        myState = 0;
        myIdx = -1;
        myType = 0;
    }

}

void MoveGpxPointFunction::b2(const QPointF &pos) {
    switch (myType) {
    case TrackPointItem::Type:
        b2_trackpoint(pos);
        break;
    case RoutePointItem::Type:
        b2_routepoint(pos);
        break;
    case WaypointItem::Type:
        b2_waypoint(pos);
    }
}

void MoveGpxPointFunction::b2_trackpoint(const QPointF &/*pos*/) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        scene->model()->changeTrackPoint(myIdx, myOldPos);
        myState = 0;
        myIdx = -1;
    }
}

void MoveGpxPointFunction::b2_routepoint(const QPointF &/*pos*/) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        double srtm = scene->model()->srtmEle(myOldPos);
        scene->model()->routePtr()->moveRoutePoint(myIdx, myOldPos, srtm);
        myState = 0;
        myIdx = -1;
    }
}

void MoveGpxPointFunction::b2_waypoint(const QPointF &/*pos*/) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        scene->model()->changeWaypoint(myIdx, myOldPos);
        myState = 0;
        myIdx = -1;
    }
}


void MoveGpxPointFunction::motion(const QPointF &pos) {
    switch (myType) {
    case TrackPointItem::Type:
        motion_trackpoint(pos);
        break;
    case RoutePointItem::Type:
        motion_routepoint(pos);
        break;
    case WaypointItem::Type:
        motion_waypoint(pos);
        break;
    }
}

void MoveGpxPointFunction::motion_trackpoint(const QPointF& pos) {
    if (myState == 1) {
        //qDebug()<<"move trackpoint "<<myIdx;
        myView->moveTrackPoint(myIdx, pos);
    }
}

void MoveGpxPointFunction::motion_routepoint(const QPointF &pos) {
    if (myState == 1) {
        myView->moveRoutePoint(myIdx, pos);
    }
}

void MoveGpxPointFunction::motion_waypoint(const QPointF &pos) {
    if (myState == 1) myView->moveWaypoint(myIdx, pos);
}

NewRoutePointFunction::NewRoutePointFunction(MapView *view) :
        ViewFunction(view)
{}

void NewRoutePointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    myView->newRoutePoint(pos);
}

DelRoutePointFunction::DelRoutePointFunction(MapView *view) :
        ViewFunction(view)
{}

void DelRoutePointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    myView->delRoutePoint(pos);
}

MoveRoutePointFunction::MoveRoutePointFunction(MapView *view) :
        ViewFunction(view),
        myState(0), myIdx(-1)
{}

void MoveRoutePointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    if (myState == 0) {
        myIdx = myView->idxOfRoutePoint(pos);
        if (myIdx >= 0) {
            MapScene *scene = static_cast<MapScene*>(myView->scene());
            myOldPos = scene->model()->route().points()->at(myIdx).coord();
            myState = 1;
        }
    }
    else {
        myState = 0;
        myIdx = -1;
    }
}

void MoveRoutePointFunction::b2(const QPointF &/*pos*/) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        double srtm = scene->model()->srtmEle(myOldPos);
        scene->model()->routePtr()->moveRoutePoint(myIdx, myOldPos, srtm);
        myState = 0;
        myIdx = -1;
    }
}

void MoveRoutePointFunction::motion(const QPointF &pos) {
    if (myState == 1) {
        myView->moveRoutePoint(myIdx, pos);
    }
}

EditRoutePointFunction::EditRoutePointFunction(MapView *view) :
    ViewFunction(view)
{}

void EditRoutePointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    myView->editRoutePoint(pos);
}

InsertRoutePointFunction::InsertRoutePointFunction(MapView *view) :
    ViewFunction(view)
{}

void InsertRoutePointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    myView->insertRoutePoint(pos);
}

NewWaypointFunction::NewWaypointFunction(MapView *view) :
    ViewFunction(view)
{}

void NewWaypointFunction::b1(const QPointF &pos, QGraphicsItem */*it*/) {
    MapScene *scene = static_cast<MapScene*>(myView->scene());
    QGraphicsTextItem *it = static_cast<QGraphicsTextItem*>(scene->functionItem());
    if (it != 0) {
        myView->newWaypoint(myPos, it->toPlainText());
        reset();
    }
    myPos = pos;
    it = new QGraphicsTextItem();
    it->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    it->setZValue(1000);
    QFont font;
    font.setPixelSize(12);
    it->setFont(font);
    it->setTextInteractionFlags(Qt::TextEditorInteraction);
    scene->setFunctionItem(it);
    it->setPos(pos);
    it->setFocus();
    qDebug()<<"text item added "<<pos;
}

bool NewWaypointFunction::key(QKeyEvent *event) {
    MapScene *scene = static_cast<MapScene*>(myView->scene());
    QGraphicsTextItem *it = static_cast<QGraphicsTextItem*>(scene->functionItem());
    if (it == 0) return false;
    if (event->key() == Qt::Key_Return) {
        myView->newWaypoint(myPos, it->toPlainText());
        reset();
        return true;
    } else if (event->key() == Qt::Key_Escape) {
        reset();
        return true;
    } else {
        return false;
    }
}

void NewWaypointFunction::reset() {
    MapScene *scene = static_cast<MapScene*>(myView->scene());
    scene->delFunctionItem();
}
