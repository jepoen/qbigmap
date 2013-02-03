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

/**
 * Compute Hessian normal form of a line.
 */

class Hesse {
private:
    double myA;
    double myB;
    double myC;
public:
    explicit Hesse(const QPointF& p0, const QPointF& p1);
    double a() const { return myA; }
    double b() const { return myB; }
    double c() const { return myC; }
    double dist(const QPointF& p) const { return myA*p.x() + myB*p.y()+myC; }
};

/**
 * Simplify a polyline using BFS
 * @returns index of preserved nodes
 */

QList<int> simplifyLine(const QPolygonF &line, const QList<bool>& stopNodes, double eps);

#endif // GEOM_H
