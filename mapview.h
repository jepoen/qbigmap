#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <memory>
#include <QGraphicsView>
#include "settings.h"

class ViewFunction;
class QAction;

class MapView : public QGraphicsView
{
    Q_OBJECT
private:
    Settings *mySettings;
    std::auto_ptr<ViewFunction> function;
    int mouseX0;
    int mouseY0;
    QGraphicsItem *tempItem;
    QAction *insertTrackPointAction;
    QAction *editTrackPointAction;
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
    int idxOfTrackPoint(const QPointF& pos);
    void moveTrackPoint(int idx, const QPointF& pos);
    void editTrackPoint(const QPointF& pos);
    void insertTrackPoint(const QPointF& pos);
    void newRoutePoint(const QPointF& pos);
    int idxOfRoutePoint(const QPointF& pos);
    void delRoutePoint(const QPointF& pos);
    void moveRoutePoint(int idx, const QPointF& pos);
    void editRoutePoint(const QPointF& pos);
    void insertRoutePoint(const QPointF& pos);
    void output(QPrinter *device, double tilesize);
    void createTempPoint(const QPointF& pos);
    void deleteTempPoint();
    void newWaypoint(const QPointF& pos, const QString& name);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
    void mouseGeoPos(const QPointF& lonLat);
public slots:
    void setShowFunction();
    void setZoomInFunction();
    void setZoomOutFunction();
    void setMoveTrackPosFunction();
    void setMoveTrackPointFunction();
    void setNewRoutePointFunction();
    void setDelRoutePointFunction();
    void setMoveRoutePointFunction();
    void setEditRoutePointFunction();
    void setInsertRoutePointFunction();
    void setNewWaypointFunction();
    void moveTempPoint(const QPointF& pos);
};

#endif // MAPVIEW_H
