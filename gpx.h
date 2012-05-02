#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QList>
#include <QString>
#include <QPointF>

class QDomElement;

class GpxPoint {
private:
    int myType;
    QPoint myCoord;
    QDateTime myTimeStamp;
    double myEle;
    QString mySym;
    QString myName;
    QString myDesc;
    QString myLink;
public:
    enum {TRK, RTE, WPT};
    explicit GpxPoint(int type, const QPointF& pos, const QDateTime& timestamp = QDateTime(), double ele = 0,
                      const QString& sym="", const QString& name="", const QString& desc="", const QString& link=""):
        myType(type), myCoord(GpxPoint::iscale(pos)), myTimeStamp(timestamp), myEle(ele), mySym(sym), myName(name), myDesc(desc), myLink(link)
    {}
    int type() const { return myType; }
    QString typeName() const;
    virtual ~GpxPoint() {}
    QPointF coord() const { return dscale(myCoord); }
    const QPoint& icoord() const { return myCoord; }
    void setCoord(const QPointF& pos) { myCoord = GpxPoint::iscale(pos); }
    const QDateTime& timestamp() const { return myTimeStamp; }
    double ele() const { return myEle; }
    const QString& sym() const { return mySym; }
    const QString& name() const { return myName; }
    const QString& desc() const { return myDesc; }
    const QString& link() const { return myLink; }
    static QPoint iscale(const QPointF& p);
    static QPointF dscale(const QPoint& p);
};

typedef QList<GpxPoint> GpxPointList;
typedef QList<GpxPointList> TrackSegList;

class TrackSegInfo {
private:
    QDateTime myStartTime;
    QDateTime myEndTime;
    int myCount;
    bool selected;
public:
    explicit TrackSegInfo(const QDateTime &startTime = QDateTime(), const QDateTime &endTime = QDateTime(),
                          int count = 0, bool sel = false) :
            myStartTime(startTime), myEndTime(endTime), myCount(count)
    {}
    const QDateTime& startTime() const { return myStartTime; }
    const QDateTime& endTime() const { return myEndTime; }
    int count() const { return myCount; }
    bool isSelected() const { return selected; }
    bool isNull() const  { return myCount == 0; }
};


class Gpx {
private:
    TrackSegList myTrackSegments;
    GpxPointList myWayPoints;
    QString myRouteName;
    GpxPointList myRoutePoints;

    void readXml(const QString& fileName);
    GpxPoint xmlParsePt(const QDomElement& el);
    GpxPointList xmlParseTrackSeg(const QDomElement& el);
public:
    Gpx(const QString& fileName);
    const TrackSegList& trackSegments() const { return myTrackSegments; }
    GpxPointList trackPoints() const;
    const GpxPointList& wayPoints() const { return myWayPoints; }
    const QString& routeName() const { return myRouteName; }
    const GpxPointList& routePoints() const { return myRoutePoints; }
    TrackSegInfo trackSegInfo(int idx) const;
    static void removeDoubles(GpxPointList& list);
};

#endif // GPX_H
