#ifndef TRACK_H
#define TRACK_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>
#include <QPair>
#include <QPoint>
#include <QRect>

class QIODevice;

class TrackPoint {
private:
    QPointF myLonLat;
    double myEle;
    QDateTime myTimeStamp;
public:
    TrackPoint(const QPointF& lonLat, double ele, const QDateTime& timestamp);
    ~TrackPoint() {}
    virtual const QPointF& lonLat() const { return myLonLat; }
    virtual void setLonLat(const QPointF& lonLat) { myLonLat = lonLat; }
    virtual double ele() const { return myEle; }
    virtual const QDateTime& timestamp() const { return myTimeStamp; }
};

class ExtTrackPoint : public TrackPoint {
private:
    double mySumDist;
    int mySumDuration;
public:
    ExtTrackPoint(const QPointF& lonLat = QPointF(), double ele = -16384, const QDateTime& timestamp = QDateTime(), double sumDist = 0,
                  double sumDuration = 0);
    virtual double sumDist() const { return mySumDist; }
    virtual void setSumDist(const double& dist) { mySumDist = dist; }
};

class TrackSeg {
private:
    bool selected;
    QList<TrackPoint> myPoints;
public:
    TrackSeg() : selected(false) {}
    TrackSeg(const QList<TrackPoint>& points);
    void setSelected(bool val) { selected = val; }
    bool isSelected() { return selected; }
    void append(const TrackPoint& point) { myPoints.append(point); }
    const QList<TrackPoint>& points() const { return myPoints; }
};

class SegInfo {
private:
    QDateTime myStartTime;
    QDateTime myEndTime;
    int myCount;
    bool selected;
public:
    SegInfo(const QDateTime& startTime, const QDateTime& endTime, int count, bool sel);
    const QDateTime& startTime() const { return myStartTime; }
    const QDateTime& endTime() const { return myEndTime; }
    int count() const { return myCount; }
    bool isSelected() { return selected; }
};

class Waypoint {
private:
    QString myName;
    QPointF myLonLat;
public:
    Waypoint(const QString& name, const QPointF& lonLat);
    const QString& name() const { return myName; }
    const QPointF& lonLat() const { return myLonLat; }
};

class BoundingBox {
private:
    QPointF myP0;
    QPointF myP1;
    QPoint myEle;
public:
    BoundingBox();
    BoundingBox(const QPointF& p, int ele);
    BoundingBox(const QPointF &p0, const QPointF& p1, const QPoint& ele);
    const QPointF& p0() const { return myP0; }
    void setP0(const QPointF& p0) { myP0 = p0; }
    const QPointF& p1() const { return myP1; }
    void setP1(const QPointF& p1) { myP1 = p1; }
    const QPoint ele() const { return myEle; }
    void setEle(const QPoint& ele) { myEle = ele; }
};

class Track
{
private:
    QString myFileName;
    bool myChanged;
    QList<TrackSeg> mySegments;
    QList<Waypoint> myWaypoints;
    QList<ExtTrackPoint> myTrackPoints;
    int myPos;
    void computeExtTrackPoints();
    void updateExtPoints();
public:
    Track(const QString& filename);
    Track(const QList<TrackPoint> trackpoints);
    const QString& fileName() {return myFileName;}
    void setFileName(const QString& filename) { myFileName = filename; }
    const bool& changed() const { return myChanged; }
    void readXml();
    void writeOrigXml(QIODevice *dev);
    void writeModifiedXml(QIODevice *dev, bool isSimple=false);
    QList<SegInfo> segmentInfo();
    void selectSegments(QList<int> indices);
    BoundingBox boundingBox() const;
    QList<TrackPoint> trackPoints() const;
    QList<Waypoint> waypoints() const { return myWaypoints;}
    QList<ExtTrackPoint> extTrackPoints() const { return myTrackPoints;}
    int pos() const { return myPos; }
    void setPos(int pos);
    int nearest(const QPointF& pos) const;
    int nearest(const QDateTime& timestamp) const;
    const ExtTrackPoint& extTrackPoint(int pos) const { return myTrackPoints.at(pos); }
    void setExtTrackPoint(int pos, const ExtTrackPoint& point);
    void delExtTrackPoint(int pos);
    double linedist(const QPointF& p0, const QPointF& p1, const QPointF& v); ///< Euklidian distance from a line
    /**
      * Simplify track at given tolerance.
      */
    Track* simplify(int tolerance);
};

#endif // TRACK_H
