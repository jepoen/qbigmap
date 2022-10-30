#include "gpxprofile.h"
#include "model.h"
#include "settings.h"
#include <QtGui>
#include <QtDebug>

void GpxProfile::paint(QPaintDevice *dev, int variant, int width, int height, int top, int textwidth) {
    QPainter painter(dev);
    painter.setFont(QFont("FreeSans", 9));
    int linespace = 11;
    painter.fillRect(0, 0, width, height, QBrush(Qt::white));
    BoundingBox bb = Gpx::boundingBox(myGpxPoints);
    width -= textwidth+5*myOffset;
    height -= top+3*myOffset;
    QPoint p0(3*myOffset, height+top+myOffset);
    QPoint pText(p0.x()+width+myOffset, myOffset+linespace);
    qDebug()<<"offset "<<myOffset<<" width "<<width<<" heigth "<<height;
    qDebug()<<"p0 "<<p0;
    double scaleX = width/bb.len();
    int ele0, ele1;
    if (variant == ELE) {
        ele0 = (bb.ele().x()/100)*100;
        ele1 = (bb.ele().y()/100+1)*100;
    } else {
        ele0 = (bb.srtm().x()/100)*100;
        ele1 = (bb.srtm().y()/100+1)*100;
    }
    double scaleY = double(height)/(ele1-ele0);
    qDebug()<<"scaleY "<<scaleY;
    painter.setPen(QPen(QBrush(Qt::black), 1));
    painter.drawLine(p0, QPoint(p0.x()+width, p0.y()));
    painter.drawLine(p0, QPoint(p0.x(), p0.y()-height));
    for (double dist = 0; dist < bb.len(); dist += 10) {
        int x = int(p0.x()+dist*scaleX);
        painter.drawLine(x, p0.y()-3, x, p0.y()+3);
        painter.drawText(QRectF(x-10, p0.y()+6, 20, 10), Qt::AlignCenter, QString("%1").arg(int(dist)));
    }
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DotLine));
    for (int ele = ele0; ele < ele1; ele += 100) {
        int y = int(p0.y()-(ele-ele0)*scaleY);
        painter.drawLine(p0.x()-1, y, p0.x()+width, y);
        painter.drawText(QRectF(p0.x()-2*myOffset-5, y-5, 2*myOffset, 10), Qt::AlignVCenter|Qt::AlignRight, QString("%1").arg(ele));
    }
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::SolidLine));
    // Fill area
    double dist = 0;
    QPolygon poly;
    poly<<p0;
    GpxPoint pOld = myGpxPoints.at(0);
    foreach (const GpxPoint& p, myGpxPoints) {
        //qDebug()<<"ele "<<p.ele()<<" ele0 "<<ele0;
        int ele = (variant == ELE) ? p.ele() : p.srtm();
        double y = p0.y()-(ele-ele0)*scaleY;
        dist += Model::geodist1(pOld.coord(), p.coord());
        double x = p0.x()+dist*scaleX;
            //qDebug()<<"("<<x<<","<<y<<")";
        poly<<QPoint(x, y);
        pOld = p;
    }
    double x = p0.x()+dist*scaleX;
    poly<<QPoint(x, p0.y());
    painter.setPen((QPen()));
    painter.setBrush((QBrush(QColor(0xcf, 0xcf, 0xff))));
    painter.drawPolygon(poly);
    // ele line
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush());
    QPainterPath path;
    bool isFirst = true;
    pOld = GpxPoint(0, QPointF(0,0));
    dist = 0.0;
    int symCount = 0;
    foreach (const GpxPoint& p, myGpxPoints) {
        //qDebug()<<"ele "<<p.ele()<<" ele0 "<<ele0;
        double x = p0.x();
        int ele = (variant == ELE) ? p.ele() : p.srtm();
        double y = p0.y()-(ele-ele0)*scaleY;
        if (isFirst) {
            path.moveTo(p0.x(), y);
            isFirst = false;
        } else {
            dist += Model::geodist1(pOld.coord(), p.coord());
            x = p0.x()+dist*scaleX;
            //qDebug()<<"("<<x<<","<<y<<")";
            path.lineTo(x, y);
        }
        if (!p.sym().isEmpty() && p.showProfile() && textwidth > 0) {
            symCount++;
            const QPixmap pix = mySettings->mapIconList().icon(p.sym()).mapIco();
            int h = height;
            painter.setPen(QPen(Qt::black, 1, Qt::DotLine));
            painter.drawLine(x, y, x, p0.y()-h+2);
            painter.setPen(Qt::black);
            //painter.drawPixmap(x-w/2, y-2*h, w, h, pix);
            //painter.drawText(x, y-2*h, QString("(%1)").arg(symCount));
            painter.drawText(x-5, p0.y()-h, QString("(%1)").arg(symCount));
            painter.drawText(pText, QString("(%1) %2 (%3m)").arg(symCount).arg(p.name()).arg(p.ele()));
            pText.setY(pText.y()+linespace);
        }
        pOld = p;
    }
    painter.setPen(QPen(QBrush(Qt::blue), 2, Qt::SolidLine));
    painter.drawPath(path);
}
