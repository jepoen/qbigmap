#ifndef TRACK_H
#define TRACK_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>
#include <QPair>
#include <QPoint>
#include <QRect>
#include <QString>
#include "gpx.h"

class QIODevice;

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
    GpxPointList myTrackPoints;
    int myPos;
    double mySumDist;
    double mySumDur;
    double mySumEleIncl;
    double mySumEleDecl;
    bool myChanged;

public:
    Track(const QList<GpxPoint>& trackpoints = QList<GpxPoint>());
    const QString& fileName() const {return myFileName;}
    void setFileName(const QString& filename) { myFileName = filename; }
    bool isEmpty() const { return myTrackPoints.size() == 0; }
    void setPoints(const GpxPointList& points);
    const bool& changed() const { return myChanged; }
    void writeOrigXml(QIODevice *dev);
    void writeModifiedXml(QIODevice *dev, bool isSimple=false);
    BoundingBox boundingBox() const;
    const GpxPointList& trackPoints() const { return myTrackPoints;}
    int pos() const { return myPos; }
    void setPos(int pos);
    double sumDist() const { return mySumDist; }
    double dist(int pos) const;
    int nearest(const QPointF& pos) const;
    int nearest(const QDateTime& timestamp) const;
    const GpxPoint& trackPoint(int pos) const { return myTrackPoints.at(pos); }
    void insertTrackPoint(int pos, const GpxPoint &point);
    void setTrackPoint(int pos, const GpxPoint &point);
    void setTrackPointPos(int pos, const QPointF& lonLat);
    void delTrackPoint(int pos);
    double linedist(const QPointF& p0, const QPointF& p1, const QPointF& v); ///< Euklidian distance from a line
    /**
      * Simplify track at given tolerance.
      */
    void clear();
    void removeDoubles() { Gpx::removeDoubles(myTrackPoints); }
    Track* simplify(int tolerance);
};

#endif // TRACK_H
