#ifndef ROUTE_H
#define ROUTE_H

#include <QObject>
#include <QPoint>
#include <QList>
#include <QString>

class QIODevice;

class RoutePoint {
private:
    QPointF myCoord;
    QString mySym;
    QString myName;
    QString myDesc;
    QString myLink;
public:
    explicit RoutePoint(const QPointF& coord, const QString& sym = "",
               const QString& name = "",
               const QString& desc = "", const QString& link = "") :
        myCoord(coord), mySym(sym), myName(name), myDesc(desc), myLink(link)
    {}
    const QPointF& coord() const { return myCoord; }
    void setCoord(const QPointF& coord) { myCoord = coord; }
    const QString& sym() const { return mySym; }
    const QString& name() const { return myName; }
    const QString& desc() const { return myDesc; }
    const QString& link() const { return myLink; }
};

typedef QList<RoutePoint> RoutePointList;

class Route : public QObject
{
    Q_OBJECT
private:
    QString myName;
    RoutePointList myPoints;
    bool myDirty;
public:
    Route(const QString& filename = QString());
    RoutePointList *points() { return &myPoints; }
    const QString name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    void newRoutePoint(const RoutePoint& point);
    void delRoutePoint(int idx);
    void updateRoutePoint(int idx, const RoutePoint& point);
    void moveRoutePoint(int idx, const QPointF& pos);
    void insertRoutePoint(int idx, const RoutePoint &point);
    void delRoute();
    bool isDirty() const { return myDirty; }
    void writeXml(QIODevice *dev);
    bool readXml(const QString& fileName);
    bool readXml(QIODevice *dev);
signals:
    void routeChanged();
    void routePointMoved(int idx);
};

#endif // ROUTE_H
