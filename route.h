#ifndef ROUTE_H
#define ROUTE_H

#include <QObject>
#include <QPoint>
#include <QList>
#include <QString>
#include "gpx.h"

class QIODevice;

class Route : public QObject
{
    Q_OBJECT
private:
    QString myFileName;
    QString myName;
    GpxPointList myPoints;
    bool myDirty;
public:
    Route(const QString& fileName="", const QString& name="", const GpxPointList& points = GpxPointList());
    const GpxPointList *points() const { return &myPoints; }
    const QString fileName() const { return myFileName; }
    void setFileName(const QString& fileName) { myFileName = fileName; }
    const QString name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    bool isEmpty() const { return myPoints.size() == 0; }
    BoundingBox boundingBox() const { return Gpx::boundingBox(myPoints); }
    void setRoutePoints(const GpxPointList& points) { myPoints = points; }
    void newRoutePoint(const GpxPoint& point);
    void delRoutePoint(int idx);
    void updateRoutePoint(int idx, const GpxPoint& point);
    void updateSrtm(int idx, int ele);
    void moveRoutePoint(int idx, const QPointF& pos, double srtm);
    void insertRoutePoint(int idx, const GpxPoint &point);
    void delRoute();
    bool isDirty() const { return myDirty; }
    void writeXml(QIODevice *dev, GpxPointList *wpts);
    void removeDoubles() { Gpx::removeDoubles(myPoints); }
signals:
    void routeChanged();
    void routePointMoved(int idx);
};

#endif // ROUTE_H
