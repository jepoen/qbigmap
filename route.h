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
    GpxPointList *points() { return &myPoints; }
    const QString fileName() const { return myFileName; }
    void setFileName(const QString& fileName) { myFileName = fileName; }
    const QString name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    void setRoutePoints(const GpxPointList& points) { myPoints = points; }
    void newRoutePoint(const GpxPoint& point);
    void delRoutePoint(int idx);
    void updateRoutePoint(int idx, const GpxPoint& point);
    void moveRoutePoint(int idx, const QPointF& pos);
    void insertRoutePoint(int idx, const GpxPoint &point);
    void delRoute();
    bool isDirty() const { return myDirty; }
    void writeXml(QIODevice *dev);
signals:
    void routeChanged();
    void routePointMoved(int idx);
};

#endif // ROUTE_H
