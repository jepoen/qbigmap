#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QList>
#include <QMap>
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
    int type() const { return Type; }
    TrackItem(const QPolygonF& points, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    const QPolygonF& points() const { return myPoints; }
    void setPoints(const QPolygonF& points);
    void setColor(const QColor col) { myColor = col; }
};

class TrackPointItem: public QGraphicsItem
{
private:
    QPointF myPoint;
    bool mySym;
public:
    enum { Type = UserType + 2 };
    TrackPointItem(const QPointF& point, const QString& sym = "", QGraphicsItem *parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPoint(const QPointF& point);
    void setSym(const QString& sym);
};

class WaypointItem : public QGraphicsItem
{
private:
    QString myText;
public:
    enum { Type = UserType + 3 };
    int type() const { return Type; }
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
    enum { Type = UserType + 4 };
    TrackPosItem(const QPointF& pos, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //void setPos(const QPointF &pos);
};

class RouteItem: public QGraphicsItem
{
private:
    QPolygonF myPoints;
public:
    enum { Type = UserType + 5 };
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
    enum { Type = UserType + 6 };
    RoutePointItem(const QPointF& point, const QString& sym = "", QGraphicsItem *parent = 0);
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPoint(const QPointF& point);
    void setSym(const QString& sym);
};

class PhotoItem: public QGraphicsItem {
private:
    QPointF myPoint;
    QPainterPath myPath;
    QRectF myBoundingRect;
    QPainterPath createPath();
public:
    enum {Type = UserType + 7};
    explicit PhotoItem(const QPointF& point, QGraphicsItem *parent = 0);
    QRectF boundingRect() const { return myBoundingRect; }
    QPainterPath shape() const { return myPath; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setPoint(const QPointF& point);
};

class MapScene : public QGraphicsScene
{
    Q_OBJECT
private:
    Model *myModel;
    TrackItem *myTrackItem;
    QList<TrackPointItem*> myTrackPointItems;
    TrackPosItem *myTrackPosItem;
    RouteItem *myRouteItem;
    QList<RoutePointItem*> myRoutePointItems;
    QList<WaypointItem*> myWaypointItems;
    QList<QPointF> myPhotoCoord; // only one entry!
    PhotoItem *myPhotoItem;
    QGraphicsItem *myTmpItem;
    QGraphicsItem *myFunctionItem;
    bool myShowGrid;
    bool myShowTileBounds;
    bool myShowTrackBb;
    QList<QGraphicsItem*> gridGroup;
    QList<QGraphicsItem*> trackGroup;
    QList<QGraphicsItem*> tileBoundGroup;
    QMap<QString, QPixmap> myPixmaps;
    HttpGet getter;
    QList<TileRequest> requests;
    QProgressDialog *progressDlg;

    void addPixmap(const QString &key, const QPixmap &pixmap, int ix, int iy, int z);
    void getNextTile();
    void redrawLayer(const Layer& layer, int z, const QMap<QString, QPixmap> &oldPixmaps);
public:
    MapScene(Model *model, QObject *parent = 0);
    Model *model() { return myModel; }
    TrackItem *trackItem() const { return myTrackItem; }
    TrackPosItem *trackPosItem() const { return myTrackPosItem; }
    QList<TrackPointItem *> trackPointItems() const { return myTrackPointItems; }
    QList<RoutePointItem *> routePointItems() const { return myRoutePointItems; }
    QList<WaypointItem *> waypointItems() const { return myWaypointItems; }
    QGraphicsItem *functionItem() const { return myFunctionItem; }
    void setFunctionItem(QGraphicsItem *it) { myFunctionItem = it; addItem(it);}
    void delFunctionItem() { delete myFunctionItem; myFunctionItem = 0; }
    void setShowGrid(bool show);
    bool isShowGrid() const { return myShowGrid; }
    void setShowTileBounds(bool show);
    bool isShowTileBounds() const { return myShowTileBounds; }
    void setShowTrackBb(bool show);
    bool isShowTrackBb() const { return myShowTrackBb; }
    void hideTrack(bool showTrackLine);
    void showTrack();
    bool isShowTrack() const { return myTrackItem != NULL; }
    QPixmap getPixmap(const QString& key) const;
    void showPhotoItem(const QPointF& pos);
    void hidePhotoItem();
    void addTmpItem(QPointF& gpos);
    void delTmpItem();
    void moveTmpItem(const QPointF& gpos);

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
    void redrawWaypoints();
    void redrawPhoto();
    void redrawTmpItem();
};

#endif // MAPSCENE_H
