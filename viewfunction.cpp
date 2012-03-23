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
            myOldPos = scene->model()->route()->points()->at(myIdx).coord();
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
        scene->model()->route()->moveRoutePoint(myIdx, myOldPos);
        myState = 0;
        myIdx = -1;
    }
}

void MoveRoutePointFunction::motion(const QPointF &pos) {
    if (myState == 1) {
        myView->moveRoutePoint(myIdx, pos);
    }
}

