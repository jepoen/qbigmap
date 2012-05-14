#include <QtXml>
#include "graph.h"
#include "model.h"
#include "track.h"

BoundingBox::BoundingBox()
{
    myEle = QPoint(-32768, -32768);
}

BoundingBox::BoundingBox(const QPointF &p, int ele) :
    myP0(p), myP1(p), myEle(ele, ele)
{}

BoundingBox::BoundingBox(const QPointF &p0, const QPointF &p1, const QPoint &ele) :
        myP0(p0), myP1(p1), myEle(ele)
{}

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

void Track::writeModifiedXml(QIODevice *dev, bool isSimple) {
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
    QDomElement trk = doc.createElement("trk");
    root.appendChild(trk);
    QDomElement name = doc.createElement("name");
    name.appendChild(doc.createTextNode("QBIGMAP"));
    trk.appendChild(name);
    QDomElement trkseg = doc.createElement("trkseg");
    trk.appendChild(trkseg);
    foreach (const GpxPoint& p, myTrackPoints) {
        QDomElement trkpt = doc.createElement("trkpt");
        trkpt.setAttribute("lon", locale.toString(p.coord().x(), 'g', 10));
        trkpt.setAttribute("lat", locale.toString(p.coord().y(), 'g', 10));
        trkseg.appendChild(trkpt);
        if (!isSimple) {
            QDomElement ele = doc.createElement("ele");
            ele.appendChild(doc.createTextNode(QString("%1").arg(p.ele())));
            trkpt.appendChild(ele);
            QDomElement timeStamp = doc.createElement("time");
            timeStamp.appendChild(doc.createTextNode(p.timestamp().toString(QString("yyyy-MM-ddThh:mm:ssZ"))));
            trkpt.appendChild(timeStamp);
        }
    }
    QTextStream stream(dev);
    doc.save(stream, 4);
}

BoundingBox Track::boundingBox() const {
    double x0 = 0;
    double x1 = 0;
    double y0 = 0;
    double y1 = 0;
    int ele0 = 0;
    int ele1 = 0;
    bool start = true;
    foreach (const GpxPoint& p, myTrackPoints) {
        if (start) {
            x1 = x0 = p.coord().x();
            y1 = y0 = p.coord().y();
            ele1 = ele0 = p.ele();
            start = false;
        }
        else {
            if (p.coord().x() < x0) x0 = p.coord().x();
            if (p.coord().x() > x1) x1 = p.coord().x();
            if (p.coord().y() < y0) y0 = p.coord().y();
            if (p.coord().y() > y1) y1 = p.coord().y();
            if (p.ele() < ele0) ele0 = p.ele();
            if (p.ele() > ele1) ele1 = p.ele();
        }
    }
    return BoundingBox(QPointF(x0, y0), QPointF(x1, y1), QPoint(ele0, ele1));
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
    i0++;
    int count = i1-i0;
    for (int i = 0; i < count; i++) myTrackPoints.removeAt(i0);
    setPos(i1);
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

Track * Track::simplify(int tolerance) {
    Graph g(myTrackPoints.size());
    for (int i = 0; i < myTrackPoints.size()-1; i++) {
        qDebug()<<"Track::simplify add edge "<<i<<" "<<i+1<<" "<<0;
        g.addEdge(Edge(i, i+1, 0));
        QPointF v0 = Model::lonLat2SpherMerc(myTrackPoints[i].coord());
        //qDebug()<<"i: "<<i<<" v0: "<<v0;
        for (int k = i+2; k < myTrackPoints.size(); k++) {
            QPointF v1 = Model::lonLat2SpherMerc(myTrackPoints[k].coord());
            //qDebug()<<"k: "<<k<<" v1 :"<<v1;
            double maxW = 0;
            for (int m = i+1; m < k; m++) {
                QPointF v = Model::lonLat2SpherMerc(myTrackPoints[m].coord());
                double w = linedist(v0, v1, v);
                if (w > maxW) maxW = w;
                if (w > tolerance) break;
            }
            //qDebug()<<"maxW: "<<maxW;
            if (maxW > 2*tolerance) break;
            int iMax = static_cast<int>(maxW+0.5);
            if (iMax < tolerance) {
                qDebug()<<"Track::simplify add edge "<<i<<" "<<k<<" "<<iMax;
                g.addEdge(Edge(i, k, iMax));
            }
        }
    }
    QList<int> idxList = g.shortestPath(0, myTrackPoints.size()-1);
    QList<GpxPoint> newPoints;
    foreach (int i, idxList) {
        //qDebug()<<"copy "<<i<<" "<<myTrackPoints[i].lonLat();
        const GpxPoint& ep = myTrackPoints[i];
        newPoints.push_back(ep);
    }
    return new Track(newPoints);
}

void Track::clear() {
    myTrackPoints.clear();
    myFileName.clear();
    myPos = -1;
    mySumDist = mySumDur = 0;
}
