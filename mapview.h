#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include "settings.h"

class ViewFunction;

class MapView : public QGraphicsView
{
    Q_OBJECT
private:
    Settings *mySettings;
    ViewFunction *function;
    int mouseX0;
    int mouseY0;
    QGraphicsItem *tempItem;

public:
    MapView(QGraphicsScene *scene, Settings *settings);
    void zoomIn(const QPointF& pos);
    void zoomOut(const QPointF& pos);
    void showPos(const QPointF& pos);
    void newRoutePoint(const QPointF& pos);
    int idxOfRoutePoint(const QPointF& pos);
    void delRoutePoint(const QPointF& pos);
    void moveRoutePoint(int idx, const QPointF& pos);
    void editRoutePoint(const QPointF& pos);
    void insertRoutePoint(const QPointF& pos);
    void output(QPrinter *device, double tilesize);
    void createTempPoint(const QPointF& pos);
    void deleteTempPoint();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:
    void mouseGeoPos(const QPointF& lonLat);
public slots:
    void setShowFunction();
    void setZoomInFunction();
    void setZoomOutFunction();
    void setNewRoutePointFunction();
    void setDelRoutePointFunction();
    void setMoveRoutePointFunction();
    void setEditRoutePointFunction();
    void setInsertRoutePointFunction();
    void moveTempPoint(const QPointF& pos);
};

#endif // MAPVIEW_H
