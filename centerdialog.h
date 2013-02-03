#ifndef CENTERDIALOG_H
#define CENTERDIALOG_H

#include <QGraphicsItem>
#include <QDialog>
#include "mapscene.h"

class QDoubleSpinBox;
class Model;

class CenterItem : public QGraphicsItem
{
public:
    CenterItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class CenterDialog : public QDialog
{
    Q_OBJECT
private:
    QPointF myLonLat;
    QDoubleSpinBox *bLon;
    QDoubleSpinBox *bLat;
    MapScene *mapScene;
    CenterItem *centerItem;

public:
    CenterDialog(const QPointF& lonLat, MapScene *scene, QWidget *parent = 0);
    void setLonLat(const QPointF& lonLat) { myLonLat = lonLat; }
    const QPointF& lonLat() const { return myLonLat; }
public slots:
    void hide();
    void changeValue(double val);
};

#endif // CENTERDIALOG_H