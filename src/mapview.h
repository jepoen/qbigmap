#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <memory>
#include <QGraphicsView>
#include <QPointF>
#include <QPrinter>
#include "settings.h"
#include "viewfunction.h"

class TrackPointItem;
class WaypointItem;
class RoutePointItem;
class QAction;

class MapView : public QGraphicsView
{
    Q_OBJECT
private:
    Settings *mySettings;
    std::unique_ptr<ViewFunction> function;
    int mouseX0;
    int mouseY0;
    QPointF myPos;
    QGraphicsItem *tempItem;
    QAction *selectTrackPointAction;
    QAction *insertTrackPointAction;
    QAction *editTrackPointAction;
    QAction *delTrackPointAction;
    QAction *editRoutePointAction;
    QAction *delRoutePointAction;
    QAction *insertRoutePointAction;
    QAction *editWaypointAction;
    QAction *delWaypointAction;

    void createActions();
public:
    MapView(QGraphicsScene *scene, Settings *settings);
    void zoomIn(const QPointF& pos);
    void zoomOut(const QPointF& pos);
    void showPos(const QPointF& pos);
    void centerView();
    int idxOfTrackPoint(const QPointF& pos);
    int idxOfTrackPoint(TrackPointItem *it);
    void moveTrackPoint(int idx, const QPointF& pos);
    void selectTrackPoint(const QPointF& pos);
    void editTrackPoint(const QPointF& pos);
    void editTrackPoint(int idx);
    void delTrackPoint(const QPointF& pos);
    void insertTrackPoint(const QPointF& pos);
    void newRoutePoint(const QPointF& pos);
    int idxOfRoutePoint(const QPointF& pos);
    int idxOfRoutePoint(RoutePointItem *it);
    void delRoutePoint(const QPointF& pos);
    void moveRoutePoint(int idx, const QPointF& pos);
    void editRoutePoint(const QPointF& pos);
    void insertRoutePoint(const QPointF& pos);
    void moveWaypoint(int idx, const QPointF& pos);
    void output(QPrinter *device, double tilesize);
    void createTempPoint(const QPointF& pos);
    void deleteTempPoint();
    int idxOfWaypoint(const QPointF& pos);
    int idxOfWaypoint(WaypointItem *it);
    void newWaypoint(const QPointF& pos, const QString& name);
    void editWaypoint(const QPointF& pos);
    void delWaypoint(const QPointF& pos);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
    void mouseGeoPos(const QPointF& lonLat);
public slots:
    void changeTrackPos(int);
    void setShowFunction();
    void setMoveTrackPosFunction();
    void setMoveGpxPointFunction();
    void delTrackPart();
    void setNewRoutePointFunction();
    void setDelRoutePointFunction();
    void setMoveRoutePointFunction();
    void setEditRoutePointFunction();
    void setInsertRoutePointFunction();
    void setNewWaypointFunction();
    void moveTempPoint(const QPointF& pos);
    void zoomInCenter();
    void zoomOutCenter();
};

#endif // MAPVIEW_H
