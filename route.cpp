#include <QIODevice>
#include <QtXml>
#include "route.h"

Route::Route(const QString& fileName) :
    myName(tr("Route")), myDirty(false)
{
    if (!fileName.isEmpty()) {
        readXml(fileName);
    }
}

void Route::newRoutePoint(const RoutePoint &point) {
    myPoints.append(point);
    myDirty = true;
    emit routeChanged();
}

void Route::delRoutePoint(int pos) {
    myPoints.removeAt(pos);
    myDirty = true;
    emit routeChanged();
}

void Route::updateRoutePoint(int idx, const RoutePoint &point) {
    myPoints[idx] = point;
    myDirty = true;
    emit routePointMoved(idx);
}

void Route::moveRoutePoint(int idx, const QPointF &pos) {
    myPoints[idx].setCoord(pos);
    myDirty = true;
    emit routePointMoved(idx);
}

void Route::insertRoutePoint(int idx, const RoutePoint &point) {
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
    foreach (const RoutePoint& p, myPoints) {
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
    }
    QTextStream stream(dev);
    doc.save(stream, 4);
    myDirty = false;
}

bool Route::readXml(const QString& fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        bool ok = readXml(&file);
        file.close();
        return ok;
    } else {
        return false;
    }
}

bool Route::readXml(QIODevice *dev) {
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorCol;
    if (!doc.setContent(dev, true, &errorStr, &errorLine, &errorCol)) {
        qDebug()<<errorStr<<errorLine<<errorCol;
        return false;
    }
    QDomElement gpx = doc.firstChildElement("gpx");
    if (gpx.isNull()) return false;
    myPoints.clear();
    QDomElement rte = gpx.firstChildElement("rte");
    QDomElement name = rte.firstChildElement("name");
    if (!name.isNull()) {
        myName = name.text();
    } else {
        myName = tr("(unknown)");
    }
    for (QDomElement pt = rte.firstChildElement("rtept");
         !pt.isNull();
         pt = pt.nextSiblingElement("rtept")) {
        double lon = pt.attribute("lon").toDouble();
        double lat = pt.attribute("lat").toDouble();
        QString sym("");
        QString name("");
        for (QDomElement c = pt.firstChildElement(); !c.isNull(); c = c.nextSiblingElement()) {
            if (c.nodeName() == "sym") {
                sym = c.text();
            } else if (c.nodeName() == "name") {
                name = c.text();
            }
        }
        RoutePoint rpt(QPointF(lon, lat), sym, name);
        myPoints.append(rpt);
    }
    emit routeChanged();
    return true;
}
