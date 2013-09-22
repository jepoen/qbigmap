#ifndef GPX_H
#define GPX_H

#include <QDateTime>
#include <QList>
#include <QString>
#include <QPointF>

class QDomElement;

class BoundingBox {
private:
    QPointF myP0;
    QPointF myP1;
    QPoint myEle;
    QPoint mySrtm;
    double myLen;
public:
    BoundingBox();
    BoundingBox(const QPointF& p, int ele, int srtm);
    BoundingBox(const QPointF &p0, const QPointF& p1, const QPoint& ele, const QPoint& srtm, double len);
    const QPointF& p0() const { return myP0; }
    void setP0(const QPointF& p0) { myP0 = p0; }
    const QPointF& p1() const { return myP1; }
    void setP1(const QPointF& p1) { myP1 = p1; }
    const QPoint ele() const { return myEle; }
    void setEle(const QPoint& ele) { myEle = ele; }
    const QPoint srtm() const { return mySrtm; }
    double len() const { return myLen; }
    QPointF center() const { return QPointF(0.5*(myP0.x()+myP1.x()), 0.5*(myP0.y()+myP1.y())); }
};

class GpxPoint {
private:
    int myType;
    QPoint myCoord;
    QDateTime myTimeStamp;
    double myEle;
    double mySrtm;
    QString mySym;
    QString myName;
    QString myDesc;
    QString myLink;
    bool myShowMap;
    bool myShowProfile;
public:
    enum {TRK, RTE, WPT};
    explicit GpxPoint(int type, const QPointF& pos, const QDateTime& timestamp = QDateTime(), double ele = 0,
                      const QString& sym="", const QString& name="", const QString& desc="", const QString& link=""):
        myType(type), myCoord(GpxPoint::iscale(pos)), myTimeStamp(timestamp), myEle(ele), mySrtm(0), mySym(sym), myName(name), myDesc(desc), myLink(link),
        myShowMap(false), myShowProfile(false)
    {
        if (type == TRK && !mySym.isEmpty()) {
            myShowMap = myShowProfile = true;
        }
    }
    int type() const { return myType; }
    QString typeName() const;
    virtual ~GpxPoint() {}
    QPointF coord() const { return dscale(myCoord); }
    const QPoint& icoord() const { return myCoord; }
    void setCoord(const QPointF& pos) { myCoord = GpxPoint::iscale(pos); }
    const QDateTime& timestamp() const { return myTimeStamp; }
    double ele() const { return myEle; }
    void setEle(int ele) { myEle = ele; }
    double srtm() const { return mySrtm; }
    void setSrtm(double srtm) { mySrtm = srtm; }
    const QString& sym() const { return mySym; }
    const QString& name() const { return myName; }
    const QString& desc() const { return myDesc; }
    const QString& link() const { return myLink; }
    bool showMap() const { return myShowMap; }
    void setShowMap(bool val) { myShowMap = val; }
    bool showProfile() const { return myShowProfile; }
    void setShowProfile(bool val) { myShowProfile = val; }
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
public:
    explicit TrackSegInfo(const QDateTime &startTime = QDateTime(), const QDateTime &endTime = QDateTime(),
                          int count = 0) :
        myStartTime(startTime), myEndTime(endTime), myCount(count)
    {}
    const QDateTime& startTime() const { return myStartTime; }
    const QDateTime& endTime() const { return myEndTime; }
    int count() const { return myCount; }
    bool isNull() const  { return myCount == 0; }
};


class Gpx {
private:
    QString myTrackName;
    TrackSegList myTrackSegments;
    GpxPointList myWayPoints;
    QString myRouteName;
    GpxPointList myRoutePoints;

    void readXml(const QString& fileName);
    GpxPoint xmlParsePt(const QDomElement& el);
    GpxPointList xmlParseTrackSeg(const QDomElement& el);
public:
    Gpx(const QString& fileName);
    bool isNull() const;
    const QString& trackName() const { return myTrackName; }
    const TrackSegList& trackSegments() const { return myTrackSegments; }
    GpxPointList trackPoints() const;
    const GpxPointList& wayPoints() const { return myWayPoints; }
    const QString& routeName() const { return myRouteName; }
    const GpxPointList& routePoints() const { return myRoutePoints; }
    TrackSegInfo trackSegInfo(int idx) const;
    static int removeDoubles(GpxPointList& list);
    static BoundingBox boundingBox(const GpxPointList& points);
    static bool hasSym(const GpxPointList& points);
};

#endif // GPX_H
