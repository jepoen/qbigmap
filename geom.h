#ifndef GEOM_H
#define GEOM_H

#include <cmath>
#include <QPolygonF>

inline double scalarprod(const QPointF& p0, const QPointF& p1) {
    return p0.x()*p1.x() + p0.y()*p1.y();
}

inline double norm(const QPointF& p) {
    return hypot(p.x(), p.y());
}

class IdxPointF {
private:
    int myIdx;
    QPointF myPoint;
    double myT;
public:
    explicit IdxPointF(int idx = -1, const QPointF& point = QPointF(), double t = 1.0) :
        myIdx(idx), myPoint(point), myT(t)
    {}
    int idx() const { return myIdx; }
    const QPointF& point() const { return myPoint; }
    double t() const { return myT; }
};

/**
  * Find the index and position along the polygon line to insert a new point pos.
  *   return.idx: insert the point before idx
  *   return.pos: insertion position on the line
  */
IdxPointF insertPoint(const QPolygonF& poly, const QPointF& pos);

#endif // GEOM_H
