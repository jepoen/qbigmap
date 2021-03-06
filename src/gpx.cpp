#include <cassert>
#include <iostream>
#include <QtXml>
#include "gpx.h"
#include "model.h"

static const int ISCALE = 10000000;

BoundingBox::BoundingBox()
{
    mySrtm = myEle = QPoint(-32768, -32768);
    myLen = 0;
}

BoundingBox::BoundingBox(const QPointF &p, int ele, int srtm) :
    myP0(p), myP1(p), myEle(ele, ele), mySrtm(srtm, srtm), myLen(0)
{}

BoundingBox::BoundingBox(const QPointF &p0, const QPointF &p1, const QPoint &ele, const QPoint& srtm, double len) :
    myP0(p0), myP1(p1), myEle(ele), mySrtm(srtm), myLen(len)
{}

void GpxPoint::swapLinks(int pos1, int pos2) {
    if (pos1 < 0 || pos1 >= myLinks.size()) return;
    if (pos2 < 0 || pos2 >= myLinks.size()) return;
    GpxLink h = myLinks[pos1];
    myLinks[pos1] = myLinks[pos2];
    myLinks[pos2] = h;
}

QPoint GpxPoint::iscale(const QPointF &p) {
    return QPoint(int(round(p.x()*ISCALE)), int(round(p.y()*ISCALE)));
}

QPointF GpxPoint::dscale(const QPoint &p) {
    return QPointF(double(p.x())/double(ISCALE), double(p.y())/double(ISCALE));
}

QString GpxPoint::typeName() const {
    switch (myType) {
    case TRK:
        return QObject::tr("trackpoint");
    case RTE:
        return QObject::tr("routepoint");
    case WPT:
        return QObject::tr("waypoint");
    }
    return QObject::tr("unknown GPX point");
}

QDomElement GpxPoint::toDomElement(QDomDocument &doc, const QString& elName, bool isSimple) const {
    QLocale locale("C");
    QDomElement domEl = doc.createElement(elName);
    domEl.setAttribute("lon", locale.toString(coord().x(), 'g', 10));
    domEl.setAttribute("lat", locale.toString(coord().y(), 'g', 10));
    if (ele() > -32768 && !isSimple) {
        QDomElement el = doc.createElement("ele");
        QDomText txt = doc.createTextNode(QString("%1").arg(ele()));
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    if (!timestamp().isNull() && !isSimple) {
        QDomElement timeStamp = doc.createElement("time");
        timeStamp.appendChild(doc.createTextNode(timestamp().toString(QString("yyyy-MM-ddThh:mm:ssZ"))));
        domEl.appendChild(timeStamp);
    }
    if (name() != "") {
        QDomElement el = doc.createElement("name");
        QDomText txt = doc.createTextNode(name());
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    if (cmt() != "") {
        QDomElement el = doc.createElement("cmt");
        QDomText txt = doc.createTextNode(cmt());
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    if (desc() != "") {
        QDomElement el = doc.createElement("desc");
        QDomText txt = doc.createTextNode(desc());
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    if (myLinks.size() > 0) {
        foreach (const GpxLink& link, myLinks) {
            if (link.url() != "") {
                QDomElement el = doc.createElement("link");
                el.setAttribute("href", link.url());
                domEl.appendChild(el);
                if (link.text() != "") {
                    QDomElement child = doc.createElement("text");
                    child.appendChild(doc.createTextNode(link.text()));
                    el.appendChild(child);
                }
                if (link.mimeType() != "") {
                    QDomElement child = doc.createElement("type");
                    child.appendChild(doc.createTextNode(link.mimeType()));
                    el.appendChild(child);
                }
            }
        }
    }
    if (sym() != "") {
        QDomElement el = doc.createElement("sym");
        QDomText txt = doc.createTextNode(sym());
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    if (gpxType() != "") {
        QDomElement el = doc.createElement("gpxType");
        QDomText txt = doc.createTextNode(gpxType());
        el.appendChild(txt);
        domEl.appendChild(el);
    }
    return domEl;
}

Gpx::Gpx(const QString &fileName) :
    myRouteName("--")
{
    readXml(fileName);
}

bool Gpx::isNull() const {
    return myRoutePoints.size() == 0 && myWayPoints.size() == 0 && myTrackSegments.size() == 0;
}

void Gpx::readXml(const QString& fileName) {
    QString errorStr;
    int errorLine, errorCol;
    QDomDocument doc;
    QFile file(fileName);

    file.open(QFile::ReadOnly);
    if (!doc.setContent(&file, true, &errorStr, &errorLine, &errorCol)) {
        qDebug()<<errorStr<<errorLine<<errorCol;
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if (root.nodeName() != "gpx") {
        std::cerr<<"No GPX file"<<std::endl;
        return;
    }
    for (QDomElement el = root.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
        if (el.nodeName() == "wpt") {
            GpxPoint pt = xmlParsePt(el);
            myWayPoints.append(pt);
        } else if (el.nodeName() == "trk") {
            for (QDomElement c = el.firstChildElement(); !c.isNull(); c = c.nextSiblingElement()) {
                if (c.nodeName() == "trkseg") {
                    GpxPointList ptl = xmlParseTrackSeg(c);
                    if (ptl.size() > 0) myTrackSegments.append(ptl);
                } else if (c.nodeName() == "name") {
                    myTrackName = c.text();
                }
            }
        } else if (el.nodeName() == "rte") {
            for (QDomElement c = el.firstChildElement(); !c.isNull(); c = c.nextSiblingElement()) {
                if (c.nodeName() == "rtept") {
                    myRoutePoints.append(xmlParsePt(c));
                } else if (c.nodeName() == "name") {
                    myRouteName = c.text();
                }
            }
        }
    }
}

GpxPointList Gpx::xmlParseTrackSeg(const QDomElement& el) {
    GpxPointList ptl;
    for (QDomElement c = el.firstChildElement(); !c.isNull(); c = c.nextSiblingElement()) {
        if (c.nodeName() != "trkpt") continue;
        ptl.append(xmlParsePt(c));
    }
    return ptl;
}

GpxPoint Gpx::xmlParsePt(const QDomElement& el) {
    double lon = el.attribute("lon", "1000").toDouble();
    double lat = el.attribute("lat", "1000").toDouble();
    double ele = -32768;
    QDateTime timestamp;
    QString sym = "";
    QString name = "";
    QString cmt = "";
    QString desc = "";
    QList<GpxLink> links;
    QString linkType = "";
    QString linkText = "";
    for (QDomElement c = el.firstChildElement(); !c.isNull(); c = c.nextSiblingElement()) {
        if (c.nodeName() == "ele") {
            ele = c.text().toDouble();
        } else if (c.nodeName() == "time") {
            timestamp = QDateTime::fromString(c.text(), "yyyy-MM-ddThh:mm:ssZ");
            timestamp.setTimeSpec(Qt::UTC);
        } else if (c.nodeName() == "sym") {
            sym = c.text();
        } else if (c.nodeName() == "name") {
            name = c.text();
        } else if (c.nodeName() == "cmt") {
            cmt = c.text();
        } else if (c.nodeName() == "desc") {
            desc = c.text();
        } else if (c.nodeName() == "link") {
            QString link = c.attribute("href", "");
            for (QDomElement cc = c.firstChildElement(); !cc.isNull(); cc = cc.nextSiblingElement()) {
                if (cc.nodeName() == "text") {
                    linkText = cc.text();
                } else if (cc.nodeName() == "type") {
                    linkType = cc.text();
                }
            }
            // Work around for wrong gpx files
            if (link.isEmpty()) link = c.text();
            links.append(GpxLink(link, linkType, linkText));
        }
    }
    int type = -1;
    if (el.nodeName() == "trkpt") type = GpxPoint::TRK;
    else if (el.nodeName() == "rtept") type = GpxPoint::RTE;
    else if (el.nodeName() == "wpt") type = GpxPoint::WPT;
    assert(type >= 0);
    return GpxPoint(type, QPointF(lon, lat), timestamp, ele, sym, name, cmt, desc, links);
}

GpxPointList Gpx::trackPoints() const {
    GpxPointList ptl;
    foreach (const GpxPointList& l, myTrackSegments) {
        foreach (const GpxPoint& p, l) {
            ptl.append(p);
        }
    }
    return ptl;
}

QStringList Gpx::symList() const {
    QStringList res;
    foreach (const GpxPointList& l, myTrackSegments) {
        foreach (const GpxPoint& p, l) {
            if (!res.contains(p.sym())) {
                res.append(p.sym());
            }
        }
    }
    foreach (const GpxPoint& p, myRoutePoints) {
        if (!res.contains(p.sym())) {
            res.append(p.sym());
        }
    }
    foreach (const GpxPoint& p, myWayPoints) {
        if (!res.contains(p.sym())) {
            res.append(p.sym());
        }
    }
    return res;
}

TrackSegInfo Gpx::trackSegInfo(int idx) const {
    if (idx < 0 || idx >= myTrackSegments.size()) return TrackSegInfo();
    const GpxPointList& ptl = myTrackSegments[idx];
    if (ptl.size() == 0) return TrackSegInfo();
    QDateTime t0 = ptl[0].timestamp();
    QDateTime t1 = ptl[ptl.size()-1].timestamp();
    return TrackSegInfo(t0, t1, ptl.size());
}

int Gpx::removeDoubles(GpxPointList &list) {
    qDebug()<<"removeDoubles";
    int cnt = 0;
    for (int i = 0; i < list.size()-1; i++) {
        if (list[i].icoord() == list[i+1].icoord()) {
            if (list[i].sym().isEmpty()) {
                list.removeAt(i);
                cnt++;
                qDebug()<<"  remove "<<i;
            } else if (list[i+1].sym().isEmpty()) {
                list.removeAt(i+1);
                cnt++;
                qDebug()<<"  remove "<<(i+1);
            } else {
                std::cerr<<"points "<<i<<" and "<<(i+1)<<" are duplicates with symbols"<<std::endl;
            }
        }
    }
    return cnt;
}
BoundingBox Gpx::boundingBox(const GpxPointList& points) {
    double x0 = 0;
    double x1 = 0;
    double y0 = 0;
    double y1 = 0;
    int ele0 = 0;
    int ele1 = 0;
    int srtm0 = 0;
    int srtm1 = 0;
    bool start = true;
    double len = 0.0;
    GpxPoint pOld(0, QPointF());
    foreach (const GpxPoint& p, points) {
        if (start) {
            x1 = x0 = p.coord().x();
            y1 = y0 = p.coord().y();
            ele1 = ele0 = p.ele();
            srtm1 = srtm0 = p.srtm();
            start = false;
        }
        else {
            if (p.coord().x() < x0) x0 = p.coord().x();
            if (p.coord().x() > x1) x1 = p.coord().x();
            if (p.coord().y() < y0) y0 = p.coord().y();
            if (p.coord().y() > y1) y1 = p.coord().y();
            if (p.ele() < ele0) ele0 = p.ele();
            if (p.ele() > ele1) ele1 = p.ele();
            if (p.srtm() < srtm0) srtm0 = p.srtm();
            if (p.srtm() > srtm1) srtm1 = p.srtm();
            len += Model::geodist1(pOld.coord(), p.coord());
        }
        pOld = p;
    }
    return BoundingBox(QPointF(x0, y0), QPointF(x1, y1), QPoint(ele0, ele1), QPoint(srtm0, srtm1), len);
}

bool Gpx::hasSym(const GpxPointList &points) {
    foreach (const GpxPoint& p, points) {
        if (!p.sym().isEmpty()) return true;
    }
    return false;
}
