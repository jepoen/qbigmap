#include <QtGui>
#include <QtDebug>
#include "gpxprofile.h"
#include "model.h"
#include "settings.h"

void GpxProfile::paint(QPaintDevice *dev, int width, int height, int textwidth) {
    QPainter painter(dev);
    painter.setFont(QFont("FreeSans", 9));
    painter.fillRect(0, 0, width, height, QBrush(Qt::yellow));
    BoundingBox bb = Gpx::boundingBox(myGpxPoints);
    width -= textwidth+4*myOffset;
    height -= 3*myOffset;
    QPoint p0(3*myOffset, height+myOffset);
    qDebug()<<"offset "<<myOffset<<" width "<<width<<" heigth "<<height;
    qDebug()<<"p0 "<<p0;
    double scaleX = width/bb.len();
    int ele0 = (bb.ele().x()/100)*100;
    int ele1 = (bb.ele().y()/100+1)*100;
    double scaleY = double(height)/(ele1-ele0);
    qDebug()<<"scaleY "<<scaleY;
    painter.setPen(QPen(QBrush(Qt::black), 1));
    painter.drawLine(p0, QPoint(p0.x()+width, p0.y()));
    painter.drawLine(p0, QPoint(p0.x(), p0.y()-height));
    for (double dist = 0; dist < bb.len(); dist += 10) {
        int x = int(p0.x()+dist*scaleX);
        painter.drawLine(x, p0.y()-5, x, p0.y()+5);
        painter.drawText(QRectF(x-10, p0.y()+6, 20, 10), Qt::AlignCenter, QString("%1").arg(int(dist)));
    }
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DotLine));
    for (int ele = ele0; ele < ele1; ele += 100) {
        int y = int(p0.y()-(ele-ele0)*scaleY);
        painter.drawLine(p0.x(), y, p0.x()+width, y);
        painter.drawText(QRectF(p0.x()-2*myOffset-5, y-5, 2*myOffset, 10), Qt::AlignVCenter|Qt::AlignRight, QString("%1").arg(ele));
    }
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::SolidLine));
    QPainterPath path;
    bool isFirst = true;
    GpxPoint pOld(0, QPointF(0,0));
    double dist = 0;
    foreach (const GpxPoint& p, myGpxPoints) {
        //qDebug()<<"ele "<<p.ele()<<" ele0 "<<ele0;
        double x = p0.x();
        double y = p0.y()-(p.ele()-ele0)*scaleY;
        if (isFirst) {
            path.moveTo(p0.x(), y);
            isFirst = false;
        } else {
            dist += Model::geodist1(pOld.coord(), p.coord());
            x = p0.x()+dist*scaleX;
            //qDebug()<<"("<<x<<","<<y<<")";
            path.lineTo(x, y);
        }
        if (!p.sym().isEmpty() && textwidth > 0) {
            const QPixmap pix = mySettings->mapIconList().icon(p.sym()).mapIco();
            int w = pix.width();
            int h = pix.height();
            painter.drawLine(x, y, x, y-3*h/2);
            painter.drawPixmap(x-w/2, y-3*h/2, w, h, pix);
        }
        pOld = p;
    }
    painter.drawPath(path);
}
