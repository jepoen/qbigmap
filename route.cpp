#include <QIODevice>
#include <QtXml>
#include "route.h"

Route::Route(const QString &fileName, const QString &name, const GpxPointList &points) :
    myFileName(fileName), myName(name), myPoints(points), myDirty(false)
{}

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

void Route::moveRoutePoint(int idx, const QPointF &pos) {
    myPoints[idx].setCoord(pos);
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
    emit routeChanged();
}

void Route::writeXml(QIODevice *dev) {
    QLocale locale("C");
    QDomDocument doc;
    QDomElement root = doc.createElement("gpx");
    root.setAttribute("xmlns", "http://www.topografix.com/GPX/1/1");
    root.setAttribute("version", "1.1");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xsi:schemaLocation", "http://www.topografix.com/GPX/1/1 gpx.xsd");
    doc.appendChild(root);
    QDomElement rte = doc.createElement("rte");
    root.appendChild(rte);
    QDomElement name = doc.createElement("name");
    QDomText txt = doc.createTextNode(myName);
    name.appendChild(txt);
    rte.appendChild(name);
    foreach (const GpxPoint& p, myPoints) {
        QDomElement rtept = doc.createElement("rtept");
        rtept.setAttribute("lon", locale.toString(p.coord().x(), 'g', 10));
        rtept.setAttribute("lat", locale.toString(p.coord().y(), 'g', 10));
        rte.appendChild(rtept);
        if (p.sym() != "") {
            QDomElement el = doc.createElement("sym");
            QDomText txt = doc.createTextNode(p.sym());
            el.appendChild(txt);
            rtept.appendChild(el);
        }
        if (p.name() != "") {
            QDomElement el = doc.createElement("name");
            QDomText txt = doc.createTextNode(p.name());
            el.appendChild(txt);
            rtept.appendChild(el);
        }
        if (p.ele() > -32768) {
            QDomElement el = doc.createElement("ele");
            QDomText txt = doc.createTextNode(QString("%1").arg(p.ele()));
            el.appendChild(txt);
            rtept.appendChild(el);
        }
        if (p.desc() != "") {
            QDomElement el = doc.createElement("desc");
            QDomText txt = doc.createTextNode(p.desc());
            el.appendChild(txt);
            rtept.appendChild(el);
        }
        if (p.link() != "") {
            QDomElement el = doc.createElement("link");
            QDomText txt = doc.createTextNode(p.link());
            el.appendChild(txt);
            rtept.appendChild(el);
        }
    }
    QTextStream stream(dev);
    doc.save(stream, 4);
    myDirty = false;
}
