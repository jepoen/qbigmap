#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QList>
#include <QTableView>
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
class QPushButton;
class QToolBar;
class QWidget;
class HelpWindow;
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
          PHOTO_TRACKIDX = Qt::UserRole+3, PHOTO_COORD = Qt::UserRole+4};
    Model *model;
    Settings settings;
    GpxListModel *myTrackPoiModel;
    QString myPhotoDir;
    int myPhotoOffset;
    MapScene *scene;
    MapView *view;
    ProfileScene *profileScene;
    ProfileView *profileView;
    QDockWidget *profileWidget;
    QDockWidget *photoWidget;
    QDockWidget *trackPoiWidget;
    QListWidget *photoList;
    QLabel *photoThumb;
    QPushButton *bPhotoOffset;
    QLabel *lPhotoOffset;
    QTableView *trackPoiListView;
    QLabel *lModelStatus;
    QLabel *lPos;
    QLabel *lTrackPos;
    QLabel *lPhotoDir;
    QMenu *mType;
    QMenu *mOverlays;
    QToolBar *toolBar;
    QToolBar *trackToolBar;
    HttpGet *getter;
    HelpWindow *myHelpWin;
    QAction *printAction;
    QAction *savePixmapAction;
    QAction *quitAction;
    QAction *loadGpxAction;
    QAction *saveGpxAction;
    QAction *trackFromGpsAction;
    QAction *loadTrackAction;
    QAction *saveTrackAction;
    QAction *mapToTrackAction;
    QAction *moveTrackPosAction;
    QAction *deleteTrackAction;
    QAction *delTrackPartAction;
    QAction *incTrackPosAction;
    QAction *dIncTrackPosAction;
    QAction *decTrackPosAction;
    QAction *dDecTrackPosAction;
    QAction *firstTrackPosAction;
    QAction *lastTrackPosAction;
    QAction *trackSimplifyAction;
    QAction *trackBoundingBoxAction;
    QAction *editTrackPointAction;
    QAction *deleteTrackPosAction;
    QAction *saveTrackProfileAction;
    QAction *newWaypointAction;
    QAction *moveGpxPointAction;
    QAction *newRoutePointAction;
    QAction *delRoutePointAction;
    QAction *moveRoutePointAction;
    QAction *editRoutePointAction;
    QAction *insertRoutePointAction;
    QAction *saveRouteAction;
    QAction *delRouteAction;
    QAction *routeAddSrtmEleAction;
    QAction *saveRouteProfileAction;
    QAction *mapToRouteAction;
    QAction *openPhotoAction;
    QAction *showPhotoAction;
    QAction *fixPhotoTimestampAction;
    QAction *geoTagPhotoAction;
    //QAction *hidePhotoAction;
    QAction *showTrackPoiAction;
    QAction *showTrackProfileAction;
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
    QAction *searchPlaceAction;
    QAction *editLayersAction;
    QAction *editOverlaysAction;
    QAction *editSettingsAction;
    QAction *helpAction;
    QPrinter *printer;

    void readSettings();
    void createPhotoWidget();
    void createActions();
    void updateTitle();
    void enableTrackActions(bool enable);
    void enablePhotoActions(bool enable);
    void createBaselayerActions();
    void createOverlayActions();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void createProfileWidget();
    void createTrackPoiTable();
    bool loadGpxFile(const QString& fileName);
    void output(QPrinter *device);
    void paintTiles(QPainter *painter, bool showOverlays);
    void paintTrack(QPainter *painter, bool showSym);
    void paintRoute(QPainter *painter, bool showSym);
    void paintWpt(QPainter *painter);
    void paintGrid(QPainter *painter);
    void paintCopy(QPainter *painter, int w, int h, const QString &copy);
    QPixmap* createPixmap();
    void connectPhotos();
    void run(const QString& path, const QStringList& params);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *evt);

signals:
    void photoDirChanged(const QString& dir);

private slots:
    void print();
    void savePixmap();
    void loadGpx();
    void readTrackFromGps();
    void saveTrack();
    void deleteTrack();
    void mapToTrack();
    GpxPointList selectTrackSegments(const Gpx& gpx);
    void incTrackPos();
    void dIncTrackPos();
    void decTrackPos();
    void dDecTrackPos();
    void firstTrackPos();
    void lastTrackPos();
    void toggleTrackBoundingBox();
    void simplifyTrack();
    void editTrackPoint();
    void deleteTrackPos();
    void saveTrackProfile();
    void saveRoute();
    void delRoute();
    void saveRouteProfile();
    void routeAddSrtmEle();
    void mapToRoute();
    void openPhotos();
    void showPhotoWidget();
    void enableShowPhoto(bool);
    //void hidePhotos();
    void selectPhotoPos(const QModelIndex& index);
    void selectPhotoPos(QListWidgetItem* item);
    void showPhotoData(QListWidgetItem *item);
    void showTrackPois(bool val);
    void showTrackProfile();
    void showPhotoDir(const QString& dir);
    void setPhotoOffset();
    void fixPhotoTimestamp();
    void geoTagPhotos();
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
    void searchPlace();
    void showImage(HttpGet *getter);
    void changeBaseLayer();
    void changeOverlays();
    void updateModelStatus();
    void changeTrackPos(int pos);
    void showGeoPos(const QPointF& pos);
    void editBaseLayers();
    void editOverlays();
    void editSettings();
    void help();
};

#endif // MAINWINDOW_H
