#include <QtGui>
#include <QtDebug>
#include "model.h"
#include "profilescene.h"

ProfileTrackItem::ProfileTrackItem(const QPolygonF &track, QGraphicsItem *parent) :
        QGraphicsItem(parent),
        myTrack(track)
{}

QRectF ProfileTrackItem::boundingRect() const {
    return myTrack.boundingRect();
}

void ProfileTrackItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawPolyline(myTrack);
}

ProfileScene::ProfileScene(Model *model, QObject *parent) :
    QGraphicsScene(parent),
    myModel(model),
    myEle0(0), myEle1(0), myWidth(0), myX0(50), myY0(0),
    myTrackPosItem(0)
{
    connect(myModel, SIGNAL(mapChanged()), this, SLOT(redrawTrack()));
    connect(myModel, SIGNAL(trackChanged()), this, SLOT(redrawTrack()));
    connect(myModel, SIGNAL(trackPosChanged(int)), this, SLOT(changeTrackPos(int)));
}

void ProfileScene::redrawTrack() {
    qDebug()<<"ProfileScene::redrawTrack";
    foreach (QGraphicsItem *it, items()) {
        delete it;
    }
    myTrackPosItem = 0;
    const Track& track = myModel->track();
    if (track.isEmpty()) return;
    const QList<GpxPoint>& points = track.trackPoints();
    int trackLen = int(ceil(Model::geodist1(points, 0, points.size()-1)));
    myWidth = (trackLen+1)*10;
    BoundingBox bb = track.boundingBox();
    int ele0 = bb.ele().x();
    int ele1 = bb.ele().y();
    myEle0 = (ele0/100)*100;
    myEle1 = (ele1/100+1)*100;
    setSceneRect(0, 0, myWidth+myX0+10, (myEle1-myEle0)+20);
    myY0 = (myEle1-myEle0)+10;
    QGraphicsItem *it = new QGraphicsLineItem(myX0, myY0, myX0, myY0-(myEle1-myEle0));
    addItem(it);
    it = new QGraphicsLineItem(myX0, myY0, myX0+myWidth, myY0);
    addItem(it);
    for (int ele = myEle0; ele <= myEle1; ele += 50) {
        it = new QGraphicsLineItem(myX0-1, myY0+myEle0-ele, myX0+1, myY0+myEle0-ele);
        addItem(it);
        if (ele % 100 == 0) {
            QGraphicsTextItem *tit = new QGraphicsTextItem(QString("%1").arg(ele));
            tit->setFont(QFont("FreeSans", 8));
            tit->setPos(10, myY0+myEle0-ele-4);
            addItem(tit);
            QGraphicsLineItem *lit = new QGraphicsLineItem(myX0, myY0+myEle0-ele, myX0+myWidth, myY0+myEle0-ele);
            lit->setPen(QPen(Qt::DotLine));
            addItem(lit);
        }
    }
    for (int dist = 100; dist <= myWidth; dist += 100) {
        it = new QGraphicsLineItem(dist+myX0, myY0-1, dist+myX0, myY0+1);
        addItem(it);
    }
    QPolygonF polygon;
    bool start = true;
    double sumDist = 0.0;
    GpxPoint pold(GpxPoint::TRK, QPointF(0,0));
    foreach (const GpxPoint& p, points) {
        if (start) {
            start = false;
        } else {
            sumDist += Model::geodist1(pold.coord(), p.coord());
        }
        QPointF pt(int(sumDist*10)+myX0, myY0+myEle0-p.ele());
        polygon.append(pt);
        pold = p;
    }
    it = new ProfileTrackItem(polygon);
    addItem(it);
}

void ProfileScene::changeTrackPos(int pos) {
    if (myModel->track().isEmpty()) return;
    if (myTrackPosItem == 0) {
        myTrackPosItem = new QGraphicsLineItem(myX0, myY0, myX0, myY0);
        myTrackPosItem->setZValue(10);
        myTrackPosItem->setPen(QPen(Qt::magenta));
        addItem(myTrackPosItem);
    }
    GpxPoint p = myModel->track().trackPoint(pos);
    double dist = myModel->track().dist(pos);
    int x = int(dist*10)+myX0;
    myTrackPosItem->setLine(x, myY0, x, myY0+myEle0-p.ele());
}
