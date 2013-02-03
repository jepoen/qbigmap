#ifndef VIEWFUNCTION_H
#define VIEWFUNCTION_H

#include <QPoint>
class QGraphicsTextItem;
class MapView;

class ViewFunction
{
protected:
    MapView *myView;
    QGraphicsItem *myIt;
public:
    ViewFunction(MapView *view);
    virtual ~ViewFunction() {}
    virtual void b1(const QPointF& pos, QGraphicsItem *it) = 0;
    virtual void b2(const QPointF& pos) {}
    virtual void motion(const QPointF& pos) {}
    virtual bool key(QKeyEvent */*event*/) { return false; }
};

class ShowFunction : public ViewFunction {
public:
    ShowFunction(MapView *view);
    void b1(const QPointF& pos, QGraphicsItem *it);
};

class ZoomInFunction : public ViewFunction {
public:
    ZoomInFunction(MapView *view);
    void b1(const QPointF& pos, QGraphicsItem *it);
};

class ZoomOutFunction : public ViewFunction {
public:
    ZoomOutFunction(MapView *view);
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
    void b2_routepoint(const QPointF& pos);
    void b2_waypoint(const QPointF& pos);
public:
    MoveGpxPointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
    void b2(const QPointF &pos);
    void motion(const QPointF &pos);
};


class NewRoutePointFunction : public ViewFunction {
public:
    NewRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
};

class DelRoutePointFunction : public ViewFunction {
public:
    DelRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
};

class MoveRoutePointFunction : public ViewFunction {
private:
    QPointF myOldPos;
    int myState;
    int myIdx;
public:
    MoveRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
    void b2(const QPointF &pos);
    void motion(const QPointF &pos);
};

class EditRoutePointFunction : public ViewFunction {
public:
    explicit EditRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
};

class InsertRoutePointFunction: public ViewFunction {
public:
    explicit InsertRoutePointFunction(MapView *view);
    void b1(const QPointF &pos, QGraphicsItem *it);
};

class NewWaypointFunction: public ViewFunction {
private:
    QGraphicsTextItem *myIt;
    QPointF myPos;

    void reset();
public:
    explicit NewWaypointFunction(MapView *view);
    ~NewWaypointFunction();
    void b1(const QPointF &pos, QGraphicsItem *it);
    bool key(QKeyEvent *);
};

#endif // VIEWFUNCTION_H
