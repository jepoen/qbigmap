#ifndef VIEWFUNCTION_H
#define VIEWFUNCTION_H

#include <QPoint>
#include "gpx.h"

class QGraphicsTextItem;
class MapView;

class ViewFunction
{
protected:
    MapView *myView;
public:
    ViewFunction(MapView *view);
    virtual ~ViewFunction() {}
    virtual void b1(const QPointF& pos, QGraphicsItem *it) = 0;
    virtual void b2(const QPointF& pos, const QPoint& globalPos, QGraphicsItem *it);
    virtual void motion(const QPointF& /*pos*/) {}
    virtual bool key(QKeyEvent */*event*/) { return false; }
    virtual GpxPoint b2_trackpoint(QGraphicsItem *it);
    virtual GpxPoint b2_routepoint(QGraphicsItem *it);
    virtual GpxPoint b2_waypoint(QGraphicsItem *it);
};

class ShowFunction : public ViewFunction {
public:
    ShowFunction(MapView *view);
    void b1(const QPointF& pos, QGraphicsItem *it);
};

class SetTrackPosFunction : public ViewFunction {
public:
    SetTrackPosFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
};

class MoveGpxPointFunction : public ViewFunction {
private:
    QPointF myOldPos;
    int myState;
    int myIdx;
    int myType;

    void b1_trackpoint(const QPointF& pos);
    void b1_routepoint(const QPointF& pos);
    void b1_waypoint(const QPointF& pos);
    void motion_trackpoint(const QPointF& pos);
    void motion_routepoint(const QPointF& pos);
    void motion_waypoint(const QPointF& pos);
    void b2_trackpoint(const QPointF& pos);
    void b2_routepoint(const QPointF& );
    void b2_waypoint(const QPointF& );
public:
    MoveGpxPointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
    void b2(const QPointF &pos, const QPoint& globalPos, QGraphicsItem *it);
    void motion(const QPointF &pos);
};


class NewRoutePointFunction : public ViewFunction {
public:
    NewRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *);
};

class DelRoutePointFunction : public ViewFunction {
public:
    DelRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *);
};

class EditRoutePointFunction : public ViewFunction {
public:
    explicit EditRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *);
};

class InsertRoutePointFunction: public ViewFunction {
public:
    explicit InsertRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *);
};

class NewWaypointFunction: public ViewFunction {
private:
    QPointF myPos;

    void reset();
public:
    explicit NewWaypointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *);
    bool key(QKeyEvent *);
};

#endif // VIEWFUNCTION_H
