#include <QtGui>
#include <QtDebug>
#include "mapscene.h"
#include "mapview.h"
#include "route.h"
#include "viewfunction.h"

ViewFunction::ViewFunction(MapView *view) :
        myView(view)
{
}

ShowFunction::ShowFunction(MapView *view) :
        ViewFunction(view)
{}

void ShowFunction::b1(const QPointF& pos) {
    myView->showPos(pos);
}

ZoomInFunction::ZoomInFunction(MapView *view) :
        ViewFunction(view)
{}

void ZoomInFunction::b1(const QPointF& pos) {
    myView->zoomIn(pos);
}

ZoomOutFunction::ZoomOutFunction(MapView *view) :
        ViewFunction(view)
{}

void ZoomOutFunction::b1(const QPointF& pos) {
    myView->zoomOut(pos);
}

SetTrackPosFunction::SetTrackPosFunction(MapView *view) :
    ViewFunction(view)
{}

void SetTrackPosFunction::b1(const QPointF &pos) {
    int idx = myView->idxOfTrackPoint(pos);
    MapScene *scene = static_cast<MapScene*>(myView->scene());
    scene->model()->setTrackPos(idx);
}

MoveTrackPointFunction::MoveTrackPointFunction(MapView *view) :
        ViewFunction(view),
        myState(0), myIdx(-1)
{}

void MoveTrackPointFunction::b1(const QPointF &pos) {
    if (myState == 0) {
        myIdx = myView->idxOfTrackPoint(pos);
        if (myIdx >= 0) {
            MapScene *scene = static_cast<MapScene*>(myView->scene());
            myOldPos = scene->model()->track().trackPoint(myIdx).coord();
            myState = 1;
        }
    }
    else {
        myState = 0;
        myIdx = -1;
    }
}

void MoveTrackPointFunction::b2(const QPointF &pos) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        scene->model()->changeTrackPoint(myIdx, myOldPos);
        myState = 0;
        myIdx = -1;
    }
}

void MoveTrackPointFunction::motion(const QPointF &pos) {
    if (myState == 1) {
        myView->moveTrackPoint(myIdx, pos);
    }
}

NewRoutePointFunction::NewRoutePointFunction(MapView *view) :
        ViewFunction(view)
{}

void NewRoutePointFunction::b1(const QPointF &pos) {
    myView->newRoutePoint(pos);
}

DelRoutePointFunction::DelRoutePointFunction(MapView *view) :
        ViewFunction(view)
{}

void DelRoutePointFunction::b1(const QPointF &pos) {
    myView->delRoutePoint(pos);
}

MoveRoutePointFunction::MoveRoutePointFunction(MapView *view) :
        ViewFunction(view),
        myState(0), myIdx(-1)
{}

void MoveRoutePointFunction::b1(const QPointF &pos) {
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

void MoveRoutePointFunction::b2(const QPointF &pos) {
    if (myIdx >= 0 && myState == 1) {
        MapScene *scene = static_cast<MapScene*>(myView->scene());
        scene->model()->routePtr()->moveRoutePoint(myIdx, myOldPos);
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

void EditRoutePointFunction::b1(const QPointF &pos) {
    myView->editRoutePoint(pos);
}

InsertRoutePointFunction::InsertRoutePointFunction(MapView *view) :
    ViewFunction(view)
{}

void InsertRoutePointFunction::b1(const QPointF &pos) {
    myView->insertRoutePoint(pos);
}

NewWaypointFunction::NewWaypointFunction(MapView *view) :
    ViewFunction(view), myIt(0)
{}

NewWaypointFunction::~NewWaypointFunction() {
    delete myIt;
}

void NewWaypointFunction::b1(const QPointF &pos) {
    if (myIt != 0) {
        myView->newWaypoint(myPos, myIt->toPlainText());
        reset();
    }
    myPos = pos;
    myIt = new QGraphicsTextItem();
    myIt->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    myIt->setZValue(1000);
    QFont font;
    font.setPixelSize(12);
    myIt->setFont(font);
    myIt->setTextInteractionFlags(Qt::TextEditorInteraction);
    myView->scene()->addItem(myIt);
    myIt->setPos(pos);
    myIt->setFocus();
    qDebug()<<"text item added "<<pos;
}

bool NewWaypointFunction::key(QKeyEvent *event) {
    if (myIt == 0) return false;
    if (event->key() == Qt::Key_Return) {
        myView->newWaypoint(myPos, myIt->toPlainText());
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
    delete myIt;
    myIt = 0;
}
