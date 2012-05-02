#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QList>
#include <QListView>
#include "gpxlistmodel.h"
#include "model.h"
#include "httpget.h"
#include "settings.h"

class QAction;
class QActionGroup;
class QDockWidget;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QPainter;
class QPrinter;
class QToolBar;
class QWidget;
class MapView;
class MapScene;
class ProfileScene;
class ProfileView;
class Model;
class Track;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum {PHOTO_FILENAME = Qt::UserRole, PHOTO_ORIGTIME = Qt::UserRole+1, PHOTO_TIMESTAMP = Qt::UserRole+2,
          PHOTO_TRACKIDX = Qt::UserRole+3};
    Settings settings;
    Model *model;
    GpxListModel *myTrackPoiModel;
    int myPhotoOffset;
    MapScene *scene;
    MapView *view;
    ProfileScene *profileScene;
    ProfileView *profileView;
    QDockWidget *photoWidget;
    QDockWidget *trackPoiWidget;
    QListWidget *photoList;
    QListView *trackPoiListView;
    QLabel *lModelStatus;
    QLabel *lPos;
    QLabel *lTrackPos;
    QMenu *mType;
    QMenu *mOverlays;
    QToolBar *toolBar;
    QToolBar *trackToolBar;
    HttpGet *getter;
    QAction *printAction;
    QAction *savePixmapAction;
    QAction *quitAction;
    QAction *loadGpxAction;
    QAction *saveGpxAction;
    QAction *trackFromGpsAction;
    QAction *loadTrackAction;
    QAction *saveTrackAction;
    QAction *moveTrackPointAction;
    QAction *moveTrackPosAction;
    QAction *deleteTrackAction;
    QAction *incTrackPosAction;
    QAction *dIncTrackPosAction;
    QAction *decTrackPosAction;
    QAction *dDecTrackPosAction;
    QAction *firstTrackPosAction;
    QAction *lastTrackPosAction;
    QAction *trackSimplifyAction;
    QAction *trackBoundingBoxAction;
    QAction *editTrackPosAction;
    QAction *deleteTrackPosAction;
    QAction *newWaypointAction;
    QAction *moveWaypointAction;
    QAction *newRoutePointAction;
    QAction *delRoutePointAction;
    QAction *moveRoutePointAction;
    QAction *editRoutePointAction;
    QAction *insertRoutePointAction;
    QAction *saveRouteAction;
    QAction *delRouteAction;
    //QAction *showPhotoAction;
    //QAction *hidePhotoAction;
    QAction *showTrackPoiAction;
    QAction *posAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *redrawAction;
    QAction *showGridAction;
    QAction *showTileBoundAction;
    QActionGroup *typeActionGroup;
    QActionGroup *ovlActionGroup;
    QAction *addNorthAction;
    QAction *addEastAction;
    QAction *addSouthAction;
    QAction *addWestAction;
    QAction *delNorthAction;
    QAction *delEastAction;
    QAction *delSouthAction;
    QAction *delWestAction;
    QAction *moveNorthAction;
    QAction *moveEastAction;
    QAction *moveSouthAction;
    QAction *moveWestAction;
    QAction *setCenterAction;
    QAction *editLayersAction;
    QAction *editOverlaysAction;
    QAction *editSettingsAction;
    QPrinter *printer;

    void readSettings();
    void createPhotoWidget();
    void createActions();
    void enableTrackActions(bool enable);
    void createBaselayerActions();
    void createOverlayActions();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void createTrackPoiTable();
    void output(QPrinter *device);
    void paintTiles(QPainter *painter, bool showOverlays);
    void paintTrack(QPainter *painter, bool showSym);
    void paintRoute(QPainter *painter, bool showSym);
    void paintGrid(QPainter *painter);
    QPixmap* createPixmap();
    void connectPhotos();
    void setPhotoOffset();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void print();
    void savePixmap();
    void loadGpx();
    void readTrackFromGps();
    void loadTrack();
    void saveTrack();
    void deleteTrack();
    GpxPointList selectTrackSegments(const Gpx& gpx);
    void incTrackPos();
    void dIncTrackPos();
    void decTrackPos();
    void dDecTrackPos();
    void firstTrackPos();
    void lastTrackPos();
    void toggleTrackBoundingBox();
    void simplifyTrack();
    void editTrackPos();
    void deleteTrackPos();
    void saveRoute();
    void delRoute();
    //void showPhotos();
    //void hidePhotos();
    //void showPhoto(QListWidgetItem * item);
    //void showPhotoData(QListWidgetItem *item);
    void showTrackPois();
    void selectTrackPoi(const QModelIndex& index);
    void toggleGrid();
    void toggleTileBounds();
    void addNorth();
    void addEast();
    void addSouth();
    void addWest();
    void delNorth();
    void delEast();
    void delSouth();
    void delWest();
    void moveNorth();
    void moveEast();
    void moveSouth();
    void moveWest();
    void setCenter();
    void showImage(HttpGet *getter);
    void changeBaseLayer();
    void changeOverlays();
    void updateModelStatus();
    void changeTrackPos(int pos);
    void showGeoPos(const QPointF& pos);
    void editBaseLayers();
    void editOverlays();
    void editSettings();
};

#endif // MAINWINDOW_H
