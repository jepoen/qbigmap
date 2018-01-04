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

class Track
{
private:
    QString myName;
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
    const QString& name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    const QString& fileName() const {return myFileName;}
    void setFileName(const QString& filename) { myFileName = filename; }
    QString simpleFileName() const;
    bool isEmpty() const { return myTrackPoints.size() == 0; }
    void setPoints(const GpxPointList& points);
    const bool& changed() const { return myChanged; }
    void writeOrigXml(QIODevice *);
    void writeModifiedXml(QIODevice *dev, const GpxPointList& waypoints, bool isSimple=false) const;
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
    void setSrtm(int pos, int ele);
    void delTrackPoint(int pos);
    void delTrackPart(int i0, int i1);
    double linedist(const QPointF& p0, const QPointF& p1, const QPointF& v); ///< Euklidian distance from a line

    /**
      * Simplify track at given tolerance.
      */
    void clear();
    int removeDoubles() { return Gpx::removeDoubles(myTrackPoints); }
};

#endif // TRACK_H
