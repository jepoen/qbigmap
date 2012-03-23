#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <QObject>
#include <QPoint>
#include <QList>

class QIODevice;

class RoutePoint {
private:
    QPointF myCoord;
    std::string mySym;
    std::string myName;
    std::string myDesc;
    std::string myLink;
public:
    RoutePoint(const QPointF& coord, const std::string& sym = "",
               const std::string& name = "",
               const std::string& desc = "", const std::string& link = "") :
        myCoord(coord), mySym(sym), myName(name), myDesc(desc), myLink(link)
    {}
    const QPointF& coord() const { return myCoord; }
    void setCoord(const QPointF& coord) { myCoord = coord; }
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
    void moveRoutePoint(int idx, const QPointF& pos);
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
