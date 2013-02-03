#ifndef VIEWFUNCTION_H
#define VIEWFUNCTION_H

#include <QPoint>
class QGraphicsTextItem;
class MapView;

class ViewFunction
{
protected:
    MapView *myView;

public:
    ViewFunction(MapView *view);
    virtual ~ViewFunction() {}
    virtual void b1(const QPointF& pos) = 0;
    virtual void b2(const QPointF& pos) {}
    virtual void motion(const QPointF& pos) {}
    virtual bool key(QKeyEvent */*event*/) { return false; }
};

class ShowFunction : public ViewFunction {
public:
    ShowFunction(MapView *view);
    void b1(const QPointF& pos);
};

class ZoomInFunction : public ViewFunction {
public:
    ZoomInFunction(MapView *view);
    void b1(const QPointF& pos);
};

class ZoomOutFunction : public ViewFunction {
public:
    ZoomOutFunction(MapView *view);
    void b1(const QPointF& pos);
};

class SetTrackPosFunction : public ViewFunction {
public:
    SetTrackPosFunction(MapView *view);
    void b1(const QPointF &pos);
};

class MoveTrackPointFunction : public ViewFunction {
private:
    QPointF myOldPos;
    int myState;
    int myIdx;
public:
    MoveTrackPointFunction(MapView *view);
    void b1(const QPointF &pos);
    void b2(const QPointF &pos);
    void motion(const QPointF &pos);
};


class NewRoutePointFunction : public ViewFunction {
public:
    NewRoutePointFunction(MapView *view);
    void b1(const QPointF &pos);
};

class DelRoutePointFunction : public ViewFunction {
public:
    DelRoutePointFunction(MapView *view);
    void b1(const QPointF &pos);
};

class MoveRoutePointFunction : public ViewFunction {
private:
    QPointF myOldPos;
    int myState;
    int myIdx;
public:
    MoveRoutePointFunction(MapView *view);
    void b1(const QPointF &pos);
    void b2(const QPointF &pos);
    void motion(const QPointF &pos);
};

class EditRoutePointFunction : public ViewFunction {
public:
    explicit EditRoutePointFunction(MapView *view);
    void b1(const QPointF &pos);
};

class InsertRoutePointFunction: public ViewFunction {
public:
    explicit InsertRoutePointFunction(MapView *view);
    void b1(const QPointF &pos);
};

class NewWaypointFunction: public ViewFunction {
private:
    QGraphicsTextItem *myIt;
    QPointF myPos;

    void reset();
public:
    explicit NewWaypointFunction(MapView *view);
    ~NewWaypointFunction();
    void b1(const QPointF &pos);
    bool key(QKeyEvent *);
};

#endif // VIEWFUNCTION_H