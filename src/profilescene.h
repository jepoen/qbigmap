#ifndef PROFILESCENE_H
#define PROFILESCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>

class QGraphicsLineItem;
class Model;
class Settings;

class ProfileTrackItem : public QGraphicsItem
{
private:
    QPolygonF myTrack;
public:
    ProfileTrackItem(const QPolygonF& track, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class ProfileScene : public QGraphicsScene
{
    Q_OBJECT
private:
    Model *myModel;
    Settings *mySettings;
    int myVariant;
    int myEle0;
    int myEle1;
    int myWidth;
    int myX0;
    int myY0;
    double myScKm;
    double myScEle;
    QGraphicsLineItem *myTrackPosItem;

public:
    enum {ELE, SRTM};
    explicit ProfileScene(Model *model, Settings *settings, QObject *parent = 0);

signals:

public slots:
    void setEle() { myVariant = ELE; redrawTrack(); }
    void setSrtm() { myVariant = SRTM; redrawTrack(); }
    void redrawTrack();
private slots:
    void changeTrackPos(int pos);
};

#endif // PROFILESCENE_H
