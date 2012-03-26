#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QList>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "httpget.h"
#include "model.h"

class QGraphicsItemGroup;
class QProgressDialog;
class QPixmap;

class TrackItem : public QGraphicsItem
{
private:
    QPolygonF myPoints;
    QColor myColor;
public:
    enum {Type = UserType + 1};
    TrackItem(const QPolygonF& points, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPoints(const QPolygonF& points);
    void setColor(const QColor col) { myColor = col; }
};

class WaypointItem : public QGraphicsItem
{
private:
    QString myText;
public:
    enum { Type = UserType + 2 };
    WaypointItem(const QString& text, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

class TrackPosItem : public QGraphicsItem
{
private:
    QPointF myPos;
public:
    enum { Type = UserType + 3 };
    TrackPosItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPos(const QPointF &pos);
};

class RouteItem: public QGraphicsItem
{
private:
    QPolygonF myPoints;
public:
    enum { Type = UserType + 4 };
    RouteItem(const QPolygonF &points, QGraphicsItem *parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    const QPolygonF& points() const { return myPoints; }
    void setPoints(const QPolygonF &points);
};

class RoutePointItem: public QGraphicsItem
{
private:
    QPointF myPoint;
    bool mySym;
public:
    enum { Type = UserType + 5 };
    RoutePointItem(const QPointF& point, const QString& sym = "", QGraphicsItem *parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPoint(const QPointF& point);
    void setSym(const QString& sym);
};

class MapScene : public QGraphicsScene
{
    Q_OBJECT
private:
    Model *myModel;
    TrackItem *myTrackItem;
    TrackPosItem *myTrackPosItem;
    RouteItem *myRouteItem;
    QList<RoutePointItem*> myRoutePointItems;
    bool myShowGrid;
    bool myShowTileBounds;
    bool myShowTrackBb;
    QList<QGraphicsItem*> gridGroup;
    QList<QGraphicsItem*> trackGroup;
    QList<QGraphicsItem*> tileBoundGroup;
    HttpGet getter;
    QList<TileRequest> requests;
    QProgressDialog *progressDlg;
    QPixmap *getPixmap(const QString& key);
    void addPixmap(QPixmap *pixmap, int ix, int iy, int z);
    void getNextTile();
    void redrawLayer(const Layer& layer, int z);
public:
    MapScene(Model *model, QObject *parent = 0);
    Model *model() { return myModel; }
    TrackItem *trackItem() const { return myTrackItem; }
    QList<RoutePointItem *> RoutePointItems() const { return myRoutePointItems; }
    void setShowGrid(bool show);
    bool isShowGrid() const { return myShowGrid; }
    void setShowTileBounds(bool show);
    bool isShowTileBounds() const { return myShowTileBounds; }
    void setShowTrackBb(bool show);
    bool isShowTrackBb() const { return myShowTrackBb; }
    void hideTrack(bool showTrackLine);
    void showTrack();
    bool isShowTrack() const { return myTrackItem != NULL; }

private slots:
    void tileLoaded(bool error);
    void cancelRequests();
public slots:
    void redraw();
    void redrawTrack();
    void changeTrackPos(int);
    void redrawGrid();
    void redrawTileBounds();
    void redrawRoute();
    void changeRoutePos(int);
};

#endif // MAPSCENE_H
