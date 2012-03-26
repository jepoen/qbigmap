#include <QtDebug>
#include "geom.h"

IdxPointF insertPoint(const QPolygonF &poly, const QPointF &pos) {
    int iMin = -1;
    double minDist2 = 0;
    QPointF res(0, 0);
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
            }
        }
    }
    return IdxPointF(iMin, res);
}
