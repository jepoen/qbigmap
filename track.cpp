#include <QtXml>
#include "graph.h"
#include "model.h"
#include "track.h"

TrackPoint::TrackPoint(const QPointF &lonLat, double ele, const QDateTime &timestamp) :
        myLonLat(lonLat), myEle(ele), myTimeStamp(timestamp)
{}

ExtTrackPoint::ExtTrackPoint(const QPointF &lonLat, double ele, const QDateTime &timestamp, double sumDist, double sumDuration) :
        TrackPoint(lonLat, ele, timestamp), mySumDist(sumDist), mySumDuration(sumDuration)
{}

TrackSeg::TrackSeg(const QList<TrackPoint> &points) :
        selected(true), myPoints(points)
{}

Waypoint::Waypoint(const QString &name, const QPointF& lonLat) :
        myName(name), myLonLat(lonLat)
{}

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

SegInfo::SegInfo(const QDateTime &startTime, const QDateTime &endTime, int count, bool sel) :
        myStartTime(startTime), myEndTime(endTime), myCount(count)
{}

Track::Track(const QString &filename) :
        myFileName(filename),
        myChanged(false)
{
    readXml();
}

Track::Track(const QList<TrackPoint> trackpoints) :
        myChanged(true)
{
    mySegments.push_back(TrackSeg(trackpoints));
    mySegments[0].setSelected(true);
    qDebug()<<"segments "<<mySegments.size()<<"0: "<<mySegments[0].points().size();
    computeExtTrackPoints();
}

void Track::readXml() {
    QString errorStr;
    int errorLine, errorCol;
    QDomDocument doc;
    QFile file(myFileName);

    file.open(QFile::ReadOnly);
    if (!doc.setContent(&file, true, &errorStr, &errorLine, &errorCol)) {
        qDebug()<<errorStr<<errorLine<<errorCol;
        file.close();
        return;
    }
    file.close();
    QDomNodeList trksegs = doc.elementsByTagName("trkseg");
    for (int i = 0; i < trksegs.count(); i++) {
        QDomElement nseg = trksegs.at(i).toElement();
        QDomNodeList trkList = nseg.elementsByTagName("trkpt");
        if (trkList.count() == 0)
            continue;
        TrackSeg seg;
        for (int k = 0; k < trkList.count(); k++) {
            QDomElement ntrkpt = trkList.at(k).toElement();
            double lon = ntrkpt.attribute("lon").toDouble();
            double lat = ntrkpt.attribute("lat").toDouble();
            double ele = 0;
            QDateTime timestamp;
            QDomNodeList nele = ntrkpt.elementsByTagName("ele");
            if (nele.count() > 0)
                ele = nele.at(0).toElement().text().toDouble();
            QDomNodeList ntimestamp = ntrkpt.elementsByTagName("time");
            if (ntimestamp.count() > 0) {
                QString stimestamp = ntimestamp.at(0).toElement().text();
                timestamp = QDateTime::fromString(stimestamp, "yyyy-MM-ddThh:mm:ssZ");
                timestamp.setTimeSpec(Qt::UTC);
            }
            seg.append(TrackPoint(QPointF(lon, lat), ele, timestamp));
        }
        seg.setSelected(true);
        mySegments.append(seg);
    }
    qDebug()<<"track segments:"<<mySegments.size();
    QDomNodeList wpts = doc.elementsByTagName("wpt");
    for (int i = 0; i < wpts.count(); i++) {
        QDomElement wpt = wpts.at(i).toElement();
        QDomElement wname = wpt.elementsByTagName("name").at(0).toElement();
        double lon = wpt.attribute("lon").toDouble();
        double lat = wpt.attribute("lat").toDouble();
        qDebug()<<"wpt: "<<wname.text()<<": "<<lon<<", "<<lat;
        myWaypoints.append(Waypoint(wname.text(), QPointF(lon, lat)));
    }
    computeExtTrackPoints();
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
    foreach (ExtTrackPoint p, myTrackPoints) {
        QDomElement trkpt = doc.createElement("trkpt");
        trkpt.setAttribute("lon", locale.toString(p.lonLat().x(), 'g', 10));
        trkpt.setAttribute("lat", locale.toString(p.lonLat().y(), 'g', 10));
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

QList<SegInfo> Track::segmentInfo() {
    QList<SegInfo> info;

    foreach(TrackSeg seg, mySegments) {
        int size = seg.points().size();
        qDebug()<<"size"<<size;
        TrackPoint p0 = seg.points().at(0);
        TrackPoint p1 = seg.points().at(size-1);
        qDebug()<<"p0 p1 read";
        QString s = QString("%1 - %2 (%3 points)").arg(p0.timestamp().toString()).arg(p1.timestamp().toString()).arg(size);
        qDebug()<<"segment: "<< s;
        info.append(SegInfo(p0.timestamp(), p1.timestamp(), size, seg.isSelected()));
    }
    return info;
}

void Track::selectSegments(QList<int>indices) {
    for(int i = 0; i < mySegments.size(); i++) {
        mySegments[i].setSelected(false);
    }
    foreach(int i, indices) {
        mySegments[i].setSelected(true);
    }
    computeExtTrackPoints();
    setPos(0);
}

BoundingBox Track::boundingBox() const {
    double x0 = 0;
    double x1 = 0;
    double y0 = 0;
    double y1 = 0;
    int ele0 = 0;
    int ele1 = 0;
    bool start = true;
    foreach (ExtTrackPoint p, myTrackPoints) {
        if (start) {
            x1 = x0 = p.lonLat().x();
            y1 = y0 = p.lonLat().y();
            ele1 = ele0 = p.ele();
            start = false;
        }
        else {
            if (p.lonLat().x() < x0) x0 = p.lonLat().x();
            if (p.lonLat().x() > x1) x1 = p.lonLat().x();
            if (p.lonLat().y() < y0) y0 = p.lonLat().y();
            if (p.lonLat().y() > y1) y1 = p.lonLat().y();
            if (p.ele() < ele0) ele0 = p.ele();
            if (p.ele() > ele1) ele1 = p.ele();
        }
    }
    return BoundingBox(QPointF(x0, y0), QPointF(x1, y1), QPoint(ele0, ele1));
}

QList<TrackPoint> Track::trackPoints() const {
    QList<TrackPoint> points;
    foreach(TrackSeg seg, mySegments) {
        if (!seg.isSelected())
            continue;
        foreach(TrackPoint p, seg.points()) {
            points.append(p);
        }
    }
    return points;
}

void Track::computeExtTrackPoints() {
    myTrackPoints.clear();
    bool isStart = true;
    ExtTrackPoint oldP;
    qDebug()<<"start Dist: "<<oldP.sumDist();
    foreach(TrackSeg seg, mySegments) {
        qDebug()<<"seg "<<seg.points().at(0).lonLat()<<" selected: "<<seg.isSelected();
        if (!seg.isSelected())
            continue;
        foreach(TrackPoint p, seg.points()) {
            if (isStart) {
                oldP = ExtTrackPoint(p.lonLat(), p.ele(), p.timestamp(), 0, 0);
                myTrackPoints.append(oldP);
                isStart = false;
                continue;
            }
            double dist = Model::geodist0(p.lonLat(), oldP.lonLat());
            oldP = ExtTrackPoint(p.lonLat(), p.ele(), p.timestamp(), oldP.sumDist()+dist, 0);
            //qDebug()<<"dist "<<dist<<" sum "<<oldP.sumDist();
            myTrackPoints.append(oldP);
        }
    }
}

void Track::updateExtPoints() {
    // TODO: update bounding box
    for (int i = 1; i < myTrackPoints.size(); i++) {
        double dist = Model::geodist0(myTrackPoints[i-1].lonLat(), myTrackPoints[i].lonLat());
        myTrackPoints[i].setSumDist(myTrackPoints[i-1].sumDist()+dist);
    }
}

void Track::setPos(int pos) {
    if (pos >= myTrackPoints.size()) pos = myTrackPoints.size()-1;
    if (pos < 0) pos = 0;
    myPos = pos;
}

int Track::nearest(const QPointF &pos) const {
    int iMin = 0;
    QPointF p = myTrackPoints[0].lonLat();
    double minDist = hypot(p.x()-pos.x(), p.y()-pos.y());
    for (int i = 1; i < myTrackPoints.size(); i++) {
        p = myTrackPoints[i].lonLat();
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

void Track::setExtTrackPoint(int pos, const ExtTrackPoint &point) {
    if (pos < 0 || pos >= myTrackPoints.size())
        return;
    myTrackPoints[pos] = point;
    updateExtPoints();
    myChanged = true;
}

void Track::delExtTrackPoint(int pos) {
    if (pos < 0 || pos >= myTrackPoints.size())
        return;
    qDebug()<<"remove "<<pos<<" size: "<<myTrackPoints.size();
    myTrackPoints.removeAt(pos);
    qDebug()<<"new size "<<myTrackPoints.size();
    updateExtPoints();
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
        QPointF v0 = Model::lonLat2SpherMerc(myTrackPoints[i].lonLat());
        //qDebug()<<"i: "<<i<<" v0: "<<v0;
        for (int k = i+2; k < myTrackPoints.size(); k++) {
            QPointF v1 = Model::lonLat2SpherMerc(myTrackPoints[k].lonLat());
            //qDebug()<<"k: "<<k<<" v1 :"<<v1;
            double maxW = 0;
            for (int m = i+1; m < k; m++) {
                QPointF v = Model::lonLat2SpherMerc(myTrackPoints[m].lonLat());
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
    QList<TrackPoint> newPoints;
    foreach (int i, idxList) {
        //qDebug()<<"copy "<<i<<" "<<myTrackPoints[i].lonLat();
        ExtTrackPoint ep = myTrackPoints[i];
        newPoints.push_back(TrackPoint(ep.lonLat(), ep.ele(), ep.timestamp()));
    }
    return new Track(newPoints);
}
