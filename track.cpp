#include <QtXml>
#include "graph.h"
#include "model.h"
#include "track.h"

Track::Track(const QList<GpxPoint>& trackpoints) :
    myTrackPoints(trackpoints), myChanged(true)
{
    removeDoubles();
    setPos(0);
}

void Track::setPoints(const GpxPointList &trackpoints) {    
    myTrackPoints = trackpoints;
    removeDoubles();
    setPos(0);
}

void Track::writeOrigXml(QIODevice *dev) {

}

void Track::writeModifiedXml(QIODevice *dev, const GpxPointList& waypoints, bool isSimple) const {
    QLocale locale("C");
    QDomDocument doc;
    QDomElement root = doc.createElement("gpx");
    root.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
    root.setAttribute("version", "1.1");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:schemaLocation", "http://www.topografix.com/GPX/1/1 gpx.xsd");
    doc.appendChild(root);
    QDomElement bounds = doc.createElement("bounds");
    BoundingBox bbox = boundingBox();
    bounds.setAttribute("minlon", locale.toString(bbox.p0().x(), 'g', 10));
    bounds.setAttribute("minlat", locale.toString(bbox.p0().y(), 'g', 10));
    bounds.setAttribute("maxlon", locale.toString(bbox.p1().x(), 'g', 10));
    bounds.setAttribute("maxlat", locale.toString(bbox.p1().y(), 'g', 10));
    root.appendChild(bounds);
    foreach (const GpxPoint& p, waypoints) {
        QDomElement wpt = doc.createElement("wpt");
        wpt.setAttribute("lon", locale.toString(p.coord().x(), 'g', 10));
        wpt.setAttribute("lat", locale.toString(p.coord().y(), 'g', 10));
        if (p.sym() != "") {
            QDomElement el = doc.createElement("sym");
            QDomText txt = doc.createTextNode(p.sym());
            el.appendChild(txt);
            wpt.appendChild(el);
        }
        if (p.name() != "") {
            QDomElement el = doc.createElement("name");
            QDomText txt = doc.createTextNode(p.name());
            el.appendChild(txt);
            wpt.appendChild(el);
        }
        if (p.desc() != "") {
            QDomElement el = doc.createElement("desc");
            QDomText txt = doc.createTextNode(p.desc());
            el.appendChild(txt);
            wpt.appendChild(el);
        }
        if (p.link() != "") {
            QDomElement el = doc.createElement("link");
            QDomText txt = doc.createTextNode(p.link());
            el.appendChild(txt);
            wpt.appendChild(el);
        }
        root.appendChild(wpt);
    }
    QDomElement trk = doc.createElement("trk");
    root.appendChild(trk);
    QDomElement name = doc.createElement("name");
    name.appendChild(doc.createTextNode(myName));
    trk.appendChild(name);
    QDomElement trkseg = doc.createElement("trkseg");
    trk.appendChild(trkseg);
    foreach (const GpxPoint& p, myTrackPoints) {
        QDomElement trkpt = doc.createElement("trkpt");
        trkpt.setAttribute("lon", locale.toString(p.coord().x(), 'g', 10));
        trkpt.setAttribute("lat", locale.toString(p.coord().y(), 'g', 10));
        trkseg.appendChild(trkpt);
        if (!isSimple) {
            if (p.ele() > -32768) {
                QDomElement ele = doc.createElement("ele");
                ele.appendChild(doc.createTextNode(QString("%1").arg(p.ele())));
                trkpt.appendChild(ele);
            }
            QDomElement timeStamp = doc.createElement("time");
            timeStamp.appendChild(doc.createTextNode(p.timestamp().toString(QString("yyyy-MM-ddThh:mm:ssZ"))));
            trkpt.appendChild(timeStamp);
        }
        if (p.sym() != "") {
            QDomElement el = doc.createElement("sym");
            QDomText txt = doc.createTextNode(p.sym());
            el.appendChild(txt);
            trkpt.appendChild(el);
        }
        if (p.name() != "") {
            QDomElement el = doc.createElement("name");
            QDomText txt = doc.createTextNode(p.name());
            el.appendChild(txt);
            trkpt.appendChild(el);
        }
        if (p.desc() != "") {
            QDomElement el = doc.createElement("desc");
            QDomText txt = doc.createTextNode(p.desc());
            el.appendChild(txt);
            trkpt.appendChild(el);
        }
        if (p.link() != "") {
            QDomElement el = doc.createElement("link");
            QDomText txt = doc.createTextNode(p.link());
            el.appendChild(txt);
            trkpt.appendChild(el);
        }
    }
    QTextStream stream(dev);
    doc.save(stream, 4);
}

BoundingBox Track::boundingBox() const {
    return Gpx::boundingBox(myTrackPoints);
}

void Track::setPos(int pos) {
    if (pos >= myTrackPoints.size()) pos = myTrackPoints.size()-1;
    if (pos < 0) pos = 0;
    myPos = pos;
    mySumDist = Model::geodist1(myTrackPoints, 0, pos);
    // TODO
    mySumDur = 0.0;
    mySumEleIncl = 0;
    mySumEleDecl = 0;
}

double Track::dist(int pos) const {
    return Model::geodist1(myTrackPoints, 0, pos);
}

int Track::nearest(const QPointF &pos) const {
    int iMin = 0;
    QPointF p = myTrackPoints[0].coord();
    double minDist = hypot(p.x()-pos.x(), p.y()-pos.y());
    for (int i = 1; i < myTrackPoints.size(); i++) {
        p = myTrackPoints[i].coord();
        double dist = hypot(p.x()-pos.x(), p.y()-pos.y());
        if (dist < minDist) {
            minDist = dist;
            iMin = i;
        }
    }
    return iMin;
}

int Track::nearest(const QDateTime &timestamp) const {
    int iMin = 0;
    qint64 minDist = llabs(timestamp.secsTo(myTrackPoints[0].timestamp()));
    for (int i = 1; i < myTrackPoints.size(); i++) {
        qint64 dist = llabs(timestamp.secsTo(myTrackPoints[i].timestamp()));
        if (dist < minDist) {
            minDist = dist;
            iMin = i;
        }
    }
    return iMin;
}

void Track::setTrackPoint(int pos, const GpxPoint &point) {
    if (pos < 0 || pos >= myTrackPoints.size())
        return;
    myTrackPoints[pos] = point;
    setPos(myPos);
    myChanged = true;
}

void Track::insertTrackPoint(int pos, const GpxPoint &point) {
    myTrackPoints.insert(pos, point);
    setPos(myPos);
    myChanged = true;
}

void Track::setTrackPointPos(int pos, const QPointF &lonLat) {
    myTrackPoints[pos].setCoord(lonLat);
    setPos(myPos);
    myChanged = true;
}

void Track::setSrtm(int pos, int ele) {
    myTrackPoints[pos].setSrtm(ele);
    myChanged = true;
}

void Track::delTrackPoint(int pos) {
    if (pos < 0 || pos >= myTrackPoints.size())
        return;
    qDebug()<<"remove "<<pos<<" size: "<<myTrackPoints.size();
    myTrackPoints.removeAt(pos);
    qDebug()<<"new size "<<myTrackPoints.size();
    if (myPos >= myTrackPoints.size()) myPos = myTrackPoints.size()-1;
    setPos(myPos);
    myChanged = true;
}

void Track::delTrackPart(int i0, int i1) {
    if (i0 >= i1 || i0 < 0 || i1 < 0 || i0 >= myTrackPoints.size() || i1 >= myTrackPoints.size()) return;
    int count = i1-i0+1;
    // don't delete complete track!
    if (count >= myTrackPoints.size()-1) return;
    qDebug()<<"delTrackPart i0 "<<i0<<" i1 "<<i1<<" count "<<count;
    for (int i = 0; i < count; i++) myTrackPoints.removeAt(i0);
    setPos(i0);
    myChanged = true;
}

double Track::linedist(const QPointF &p0, const QPointF &p1, const QPointF &v) {
    QPointF v0(p1-p0);
    QPointF v1(v-p0);
    double t = (v0.x()*v1.x() + v0.y()*v1.y())/(v0.x()*v0.x()+v0.y()*v0.y());
    if (t > 1.0) {
        v1 = v-p1;
    }
    else if (t > 0.0) {
        QPointF h(t*v0);
        v1 = v1-h;
    }
    //qDebug()<<"t "<<t<<" dist "<<v1;
    return hypot(v1.x(), v1.y());
}

void Track::clear() {
    myTrackPoints.clear();
    myFileName.clear();
    myPos = -1;
    mySumDist = mySumDur = 0;
}

QString Track::simpleFileName() const {
    int pos = myFileName.lastIndexOf(".");
    if (pos >= 0) {
        return myFileName.left(pos)+"_s.gpx";
    }
    else {
        return myFileName+"_s.gpx";
    }
}
