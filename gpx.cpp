#include <cassert>
#include <iostream>
#include <QtXml>
#include "gpx.h"

static const int ISCALE = 10000000;

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

Gpx::Gpx(const QString &fileName) :
    myRouteName("--")
{
    readXml(fileName);
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
    QString desc = "";
    QString link = "";
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
        } else if (c.nodeName() == "desc") {
            desc = c.text();
        } else if (c.nodeName() == "link") {
            link = c.text();
        }
    }
    int type = -1;
    if (el.nodeName() == "trkpt") type = GpxPoint::TRK;
    else if (el.nodeName() == "rtept") type = GpxPoint::RTE;
    else if (el.nodeName() == "wpt") type = GpxPoint::WPT;
    assert(type >= 0);
    return GpxPoint(type, QPointF(lon, lat), timestamp, ele, sym, name, desc, link);
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

TrackSegInfo Gpx::trackSegInfo(int idx) const {
    if (idx < 0 || idx >= myTrackSegments.size()) return TrackSegInfo();
    const GpxPointList& ptl = myTrackSegments[idx];
    if (ptl.size() == 0) return TrackSegInfo();
    QDateTime t0 = ptl[0].timestamp();
    QDateTime t1 = ptl[ptl.size()-1].timestamp();
    return TrackSegInfo(t0, t1, ptl.size(), true);
}

void Gpx::removeDoubles(GpxPointList &list) {
    qDebug()<<"removeDoubles";
    for (int i = 0; i < list.size()-1; i++) {
        if (list[i].icoord() == list[i+1].icoord()) {
            if (list[i].sym().isEmpty()) {
                list.removeAt(i);
                qDebug()<<"  remove "<<i;
            } else if (list[i+1].sym().isEmpty()) {
                list.removeAt(i+1);
                qDebug()<<"  remove "<<(i+1);
            } else {
                std::cerr<<"points "<<i<<" and "<<(i+1)<<" are duplicates with symbols"<<std::endl;
            }
        }
    }
}
