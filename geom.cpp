#include <QtDebug>
#include "geom.h"
#include "graph.h"

IdxPointF insertPoint(const QPolygonF &poly, const QPointF &pos) {
    int iMin = -1;
    double minDist2 = 0;
    QPointF res(0, 0);
    double minT = 1.0;
    for (int i = 1; i < poly.size(); i++) {
        QPointF vl = poly.at(i) - poly.at(i-1);
        QPointF vx = pos - poly.at(i-1);
        double splx = scalarprod(vx, vl);
        double spll = scalarprod(vl, vl);
        double t = splx/spll;
        qDebug()<<"insertPoint i: "<<i<<" t: "<<t;
        if (t > 0.0 && t < 1.0) {
            double dist2 = scalarprod(vx, vx) - splx*splx/spll;
            if (iMin <= 0 || dist2 < minDist2) {
                iMin = i;
                minDist2 = dist2;
                res = QPointF((1.0-t)*poly.at(i-1) + t*poly.at(i));
                minT = t;
            }
        }
    }
    return IdxPointF(iMin, res, minT);
}

Hesse::Hesse(const QPointF &p0, const QPointF &p1) {
    myA = p1.y()-p0.y();
    myB = p0.x()-p1.x();
    myC = p1.x()*p0.y() - p0.x()*p1.y();
    double q = sqrt(myA*myA + myB*myB);
    if (fabs(q) > 1e-10) {
        myA /= q;
        myB /= q;
        myC /= q;
    }
}

QList<int> simplifyLine(const QPolygonF& line, const QList<bool>& stopNodes, double eps) {
    Graph g(line.size());
    qDebug()<<"simplifyLine "<<line.size()<<" eps: "<<eps;
    for (int i = 1; i < line.size(); i++) {
        g.addEdge(Edge(i-1, i, 0));
        for (int k = i+1; k < line.size(); k++) {
            if (stopNodes[k-1]) break;
            Hesse h(line[i-1], line[k]);
            double dist = 3*eps;
            for (int m = i; m < k; m++) {
                dist = fabs(h.dist(line[m]));
                if (dist > eps) break;
            }
            //qDebug()<<"from "<<i-1<<" to "<<k<<" dist "<<dist<<" eps "<<eps;
            if (dist < eps) g.addEdge(Edge(i-1, k, dist));
            if (dist > 2*eps) break;
        }
    }
    return g.shortestPath(0, line.size()-1);
}
