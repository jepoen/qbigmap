#include <QIODevice>
#include <QtXml>
#include "route.h"

Route::Route(const QString &fileName, const QString &name, const GpxPointList &points) :
    myFileName(fileName), myName(name), myPoints(points), myDirty(false)
{}

GpxPoint Route::routePoint(int idx) const {
    if (idx >= 0 && idx < myPoints.size()) return myPoints[idx]; else return GpxPoint();
}

void Route::newRoutePoint(const GpxPoint &point) {
    myPoints.append(point);
    myDirty = true;
    emit routeChanged();
}

void Route::delRoutePoint(int pos) {
    myPoints.removeAt(pos);
    myDirty = true;
    emit routeChanged();
}

void Route::updateRoutePoint(int idx, const GpxPoint &point) {
    myPoints[idx] = point;
    myDirty = true;
    emit routePointMoved(idx);
}

void Route::updateSrtm(int idx, int ele) {
    myPoints[idx].setSrtm(ele);
    myPoints[idx].setEle(ele);
}

void Route::moveRoutePoint(int idx, const QPointF &pos, double srtm) {
    myPoints[idx].setCoord(pos);
    myPoints[idx].setSrtm(srtm);
    myDirty = true;
    emit routePointMoved(idx);
}

void Route::insertRoutePoint(int idx, const GpxPoint &point) {
    myPoints.insert(idx, point);
    myDirty = true;
    emit routeChanged();
}

void Route::delRoute() {
    myPoints.clear();
    myDirty = false;
    myFileName.clear();
    emit routeChanged();
}

void Route::reverseRoute() {
    for (int i = 0, k = myPoints.size()-1; i < k; i++, k--) {
        //myPoints.swapItemsAt(i, k);
        GpxPoint h = myPoints[i];
        myPoints[i] = myPoints[k];
        myPoints[k] = h;
    }
    emit routeChanged();
}

void Route::writeXmlWpt(QDomDocument& doc, GpxPointList *wpts) const {
    QDomElement root = doc.documentElement();
    foreach (const GpxPoint&p, *wpts) {
        QDomElement wpt = p.toDomElement(doc, "wpt", false);
        root.appendChild(wpt);
    }
}

void Route::writeXmlRte(QDomDocument &doc) const {
    QDomElement root = doc.documentElement();
    QDomElement rte = doc.createElement("rte");
    root.appendChild(rte);
    QDomElement name = doc.createElement("name");
    QDomText txt = doc.createTextNode(myName);
    name.appendChild(txt);
    rte.appendChild(name);
    foreach (const GpxPoint& p, myPoints) {
        QDomElement rtept = p.toDomElement(doc, "rtept", false);
        rte.appendChild(rtept);
    }
}

void Route::writeXmlTrk(QDomDocument &doc) const {
    QLocale locale("C");
    QDomElement root = doc.documentElement();
    QDomElement trk = doc.createElement("trk");
    root.appendChild(trk);
    QDomElement name = doc.createElement("name");
    QDomText txt = doc.createTextNode(myName);
    name.appendChild(txt);
    trk.appendChild(name);
    QDomElement trkseg = doc.createElement("trkseg");
    trk.appendChild(trkseg);
    foreach (const GpxPoint& p, myPoints) {
        QDomElement trkpt = p.toDomElement(doc, "trkpt", true);
        trkpt.setAttribute("lon", locale.toString(p.coord().x(), 'g', 10));
        trkpt.setAttribute("lat", locale.toString(p.coord().y(), 'g', 10));
        trkseg.appendChild(trkpt);
    }
}

void Route::writeXml(QIODevice *dev, GpxPointList *wpts, bool asTrack) {
    QDomDocument doc;
    QDomElement root = doc.createElement("gpx");
    root.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
    root.setAttribute("version", "1.1");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:schemaLocation", "http://www.topografix.com/GPX/1/1 gpx.xsd");
    root.setAttribute("creator", "qbigmap");
    doc.appendChild(root);
    if (wpts != 0) writeXmlWpt(doc, wpts);
    if (asTrack) {
        writeXmlTrk(doc);
    } else {
        writeXmlRte(doc);
    }
    QTextStream stream(dev);
    doc.save(stream, 4);
    myDirty = false;
}
