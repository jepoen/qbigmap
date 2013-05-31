#include <QtGui>
#include <QtDebug>
#include "centerdialog.h"
#include "gpxprofile.h"
#include "gpxprofiledlg.h"
#include "helpwindow.h"
#include "mainwindow.h"
#include "model.h"
#include "mapscene.h"
#include "mapview.h"
#include "profilescene.h"
#include "profileview.h"
#include "layersdialog.h"
#include "outputseldlg.h"
#include "osmmap.h"
#include "photo.h"
#include "photodlg.h"
#include "photooffsetdlg.h"
#include "placedialog.h"
#include "saveroutedlg.h"
#include "settingsdialog.h"
#include "track.h"
#include "route.h"
#include "trackposdlg.h"
#include "trackseldialog.h"
#include "trackexportdlg.h"
#include "tracksimplifydlg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      myPhotoDir(QFileInfo(".").absoluteFilePath()),
      myPhotoOffset(0)
{
    qDebug()<<"1";
    readSettings();
    qDebug()<<"2";
    myTrackPoiModel = new GpxListModel(model, &settings.mapIconList());
    scene = new MapScene(model);
    scene->setSceneRect(0, 0, 256*model->width(), 256*model->height());
    view = new MapView(scene, &settings);
    createActions();
    createMenuBar();
    createToolBar();
    createStatusBar();
    printer = new QPrinter();
    myHelpWin = new HelpWindow(this);
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(view, 1, 1);
    QHBoxLayout *northLayout = new QHBoxLayout();
    northLayout->addStretch(1);
    QToolButton *bAddNorth = new QToolButton();
    bAddNorth->setDefaultAction(addNorthAction);
    northLayout->addWidget(bAddNorth);
    QToolButton *bMoveNorth = new QToolButton();
    bMoveNorth->setDefaultAction(moveNorthAction);
    northLayout->addWidget(bMoveNorth);
    QToolButton *bDelNorth = new QToolButton();
    bDelNorth->setDefaultAction(delNorthAction);
    northLayout->addWidget(bDelNorth);
    northLayout->addStretch(1);
    mainLayout->addLayout(northLayout, 0, 0, 1, 3);
    QVBoxLayout *westLayout = new QVBoxLayout();
    westLayout->addStretch(1);
    QToolButton *bAddWest = new QToolButton();
    bAddWest->setDefaultAction(addWestAction);
    westLayout->addWidget(bAddWest);
    QToolButton *bMoveWest = new QToolButton();
    bMoveWest->setDefaultAction(moveWestAction);
    westLayout->addWidget(bMoveWest);
    QToolButton *bDelWest = new QToolButton();
    bDelWest->setDefaultAction(delWestAction);
    westLayout->addWidget(bDelWest);
    westLayout->addStretch(1);
    mainLayout->addLayout(westLayout, 1, 0);
    QVBoxLayout *eastLayout = new QVBoxLayout();
    eastLayout->addStretch(1);
    QToolButton *bAddEast = new QToolButton();
    bAddEast->setDefaultAction(addEastAction);
    eastLayout->addWidget(bAddEast);
    QToolButton *bMoveEast = new QToolButton();
    bMoveEast->setDefaultAction(moveEastAction);
    eastLayout->addWidget(bMoveEast);
    QToolButton *bDelEast = new QToolButton();
    bDelEast->setDefaultAction(delEastAction);
    eastLayout->addWidget(bDelEast);
    eastLayout->addStretch(1);
    mainLayout->addLayout(eastLayout, 1, 2);
    QHBoxLayout *southLayout = new QHBoxLayout();
    southLayout->addStretch(1);
    QToolButton *bAddSouth = new QToolButton();
    bAddSouth->setDefaultAction(addSouthAction);
    southLayout->addWidget(bAddSouth);
    QToolButton *bMoveSouth = new QToolButton();
    bMoveSouth->setDefaultAction(moveSouthAction);
    southLayout->addWidget(bMoveSouth);
    QToolButton *bDelSouth = new QToolButton();
    bDelSouth->setDefaultAction(delSouthAction);
    southLayout->addWidget(bDelSouth);
    southLayout->addStretch(1);
    mainLayout->addLayout(southLayout, 2, 0, 1, 3);
    QWidget *center = new QWidget();
    center->setLayout(mainLayout);
    createProfileWidget();
    addDockWidget(Qt::BottomDockWidgetArea, profileWidget, Qt::Horizontal);
    trackPoiWidget = new QDockWidget(tr("Track POIs"));
    createTrackPoiTable();
    addDockWidget(Qt::RightDockWidgetArea, trackPoiWidget, Qt::Vertical);
    createPhotoWidget();
    //horizontalSplitter->addWidget(photoWidget);
    addDockWidget(Qt::RightDockWidgetArea, photoWidget, Qt::Vertical);
    setCentralWidget(center);
    createBaselayerActions();
    createOverlayActions();
    enableTrackActions(false);
    enablePhotoActions(false);
    // should draw the scene
    changeOverlays();
    toggleGrid();
    connect(model, SIGNAL(mapChanged()), this, SLOT(updateModelStatus()));
    connect(model, SIGNAL(trackPosChanged(int)), this, SLOT(changeTrackPos(int)));
    connect(model, SIGNAL(trackPosChanged(int)), view, SLOT(changeTrackPos(int)));
    connect(view, SIGNAL(mouseGeoPos(QPointF)), this, SLOT(showGeoPos(QPointF)));
    connect(photoList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(showPhotoData(QListWidgetItem*)));
    connect(photoList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showPhotoData(QListWidgetItem*)));
    connect(photoList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectPhotoPos(QListWidgetItem*)));
    connect(bPhotoOffset, SIGNAL(clicked()), this, SLOT(setPhotoOffset()));
    connect(trackPoiListView, SIGNAL(activated(QModelIndex)), this, SLOT(selectTrackPoi(QModelIndex)));
    connect(model, SIGNAL(trackChanged()), myTrackPoiModel, SLOT(updatePointList()));
    updateModelStatus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::readSettings() {
    settings.load(this);
    model = new Model(settings);
}

void MainWindow::createPhotoWidget() {
    photoWidget = new QDockWidget(tr("Photos"));
    QWidget *content = new QWidget();
    QGridLayout *photoLayout = new QGridLayout();
    photoList = new QListWidget();
    //photoList->setViewMode(QListView::IconMode);
    //photoList->setIconSize(QSize(200, 200));
    //QLabel *lText = new QLabel(tr("Test"));
    //photoLayout->addWidget(lText, 0, 0);
    photoList->setMinimumWidth(180);
    photoLayout->addWidget(photoList, 0, 0, 1, 2);
    bPhotoOffset = new QPushButton(tr("+/-"));
    bPhotoOffset->setToolTip(tr("Time Offet between track and photos"));
    photoLayout->addWidget(bPhotoOffset, 1, 0);
    lPhotoOffset = new QLabel(QString("%1").arg(myPhotoOffset));
    photoLayout->addWidget(lPhotoOffset);
    photoThumb = new QLabel();
    photoThumb->setMinimumHeight(200);
    photoThumb->setAlignment(Qt::AlignCenter);
    photoLayout->addWidget(photoThumb, 2, 0, 1, 2);
    content->setLayout(photoLayout);
    photoWidget->setWidget(content);
    photoWidget->setVisible(false);
    connect(photoWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(enableShowPhoto(bool)));
    //connect(this, SIGNAL(photoDirChanged(QString)), this, SLOT(showPhotoDir(QString)));
}

void MainWindow::createActions() {
    QActionGroup *functionActionGroup = new QActionGroup(this);
    printAction = new QAction(tr("&Print..."), this);
    printAction->setIcon(QIcon(":/icons/printer.png"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));
    savePixmapAction = new QAction(tr("Save pixmap..."), this);
    connect(savePixmapAction, SIGNAL(triggered()), this, SLOT(savePixmap()));
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    loadGpxAction = new QAction(tr("Read GPX file..."), this);
    connect(loadGpxAction, SIGNAL(triggered()), this, SLOT(loadGpx()));
    saveGpxAction = new QAction(tr("Save GPX file ..."), this);
    trackFromGpsAction = new QAction(tr("Read GPS device"), this);
    connect(trackFromGpsAction, SIGNAL(triggered()), this, SLOT(readTrackFromGps()));
    loadTrackAction = new QAction(tr("Load track..."), this);
    connect(loadTrackAction, SIGNAL(triggered()), this, SLOT(loadGpx()));
    saveTrackAction = new QAction(QIcon(":/icons/disk.png"), tr("Save..."), this);
    connect(saveTrackAction, SIGNAL(triggered()), this, SLOT(saveTrack()));
    moveTrackPosAction = new QAction(tr("Select Track Position"), functionActionGroup);
    moveTrackPosAction->setCheckable(true);
    connect(moveTrackPosAction, SIGNAL(triggered()), view, SLOT(setMoveTrackPosFunction()));
    deleteTrackAction = new QAction(tr("Delete track"), this);
    firstTrackPosAction = new QAction(QIcon(":/icons/resultset_first.png"), tr("First Track position"), this);
    connect(firstTrackPosAction, SIGNAL(triggered()), this, SLOT(firstTrackPos()));
    lastTrackPosAction = new QAction(QIcon(":/icons/resultset_last.png"), tr("Last Track position"), this);
    incTrackPosAction = new QAction(QIcon(":/icons/resultset_next.png"), tr("Increment Track position"), this);
    connect(incTrackPosAction, SIGNAL(triggered()), this, SLOT(incTrackPos()));
    dIncTrackPosAction = new QAction(QIcon(":/icons/dnext.png"), tr("Fast increment track position"), this);
    connect(dIncTrackPosAction, SIGNAL(triggered()), this, SLOT(dIncTrackPos()));
    decTrackPosAction = new QAction(QIcon(":/icons/resultset_previous.png"), tr("Decrement Track position"), this);
    connect(decTrackPosAction, SIGNAL(triggered()), this, SLOT(decTrackPos()));
    dDecTrackPosAction = new QAction(QIcon(":/icons/dprev.png"), tr("Fast decrement Track positiion"), this);
    connect(dDecTrackPosAction, SIGNAL(triggered()), this, SLOT(dDecTrackPos()));
    trackBoundingBoxAction = new QAction(tr("Show Track Box"), this);
    trackBoundingBoxAction->setCheckable(true);
    connect(trackBoundingBoxAction, SIGNAL(triggered()), this, SLOT(toggleTrackBoundingBox()));
    trackSimplifyAction = new QAction(tr("Simplify track..."), this);
    connect(trackSimplifyAction, SIGNAL(triggered()), this, SLOT(simplifyTrack()));
    editTrackPointAction = new QAction(tr("&Edit track point..."), this);
    connect(editTrackPointAction, SIGNAL(triggered()), this, SLOT(editTrackPoint()));
    deleteTrackPosAction = new QAction(tr("&Delete track position"), this);
    connect(deleteTrackPosAction, SIGNAL(triggered()), this, SLOT(deleteTrackPos()));
    delTrackPartAction = new QAction(tr("Delete track part..."), this);
    connect(delTrackPartAction, SIGNAL(triggered()), view, SLOT(delTrackPart()));
    saveTrackProfileAction = new QAction(tr("Save track profile..."), this);
    connect(saveTrackProfileAction, SIGNAL(triggered()), this, SLOT(saveTrackProfile()));
    redrawAction = new QAction(tr("&Redraw"), this);
    redrawAction->setIcon(QIcon(":/icons/arrow_refresh.png"));
    zoomInAction = new QAction(QIcon(":/icons/zoom_in.png"), tr("Zoom in"), this);
    connect(zoomInAction, SIGNAL(triggered()), view, SLOT(zoomInCenter()));
    //zoomInAction->setCheckable(true);
    //functionActionGroup->addAction(zoomInAction);
    zoomOutAction = new QAction(QIcon(":/icons/zoom_out.png"), tr("Zoom out"), this);
    connect(zoomOutAction, SIGNAL(triggered()), view, SLOT(zoomOutCenter()));
    //zoomOutAction->setCheckable(true);
    //functionActionGroup->addAction(zoomOutAction);
    newWaypointAction = new QAction(tr("New waypoint"), functionActionGroup);
    newWaypointAction->setCheckable(true);
    connect(newWaypointAction, SIGNAL(triggered()), view, SLOT(setNewWaypointFunction()));
    newRoutePointAction = new QAction(tr("New route point"), functionActionGroup);
    newRoutePointAction->setCheckable(true);
    connect(newRoutePointAction, SIGNAL(triggered()), view, SLOT(setNewRoutePointFunction()));
    moveGpxPointAction = new QAction(tr("Move Point"), functionActionGroup);
    moveGpxPointAction->setCheckable(true);
    connect(moveGpxPointAction, SIGNAL(triggered()), view, SLOT(setMoveGpxPointFunction()));
    delRoutePointAction = new QAction(tr("Delete route point"), this);
    delRoutePointAction->setCheckable(true);
    functionActionGroup->addAction(delRoutePointAction);
    connect(delRoutePointAction, SIGNAL(triggered()), view, SLOT(setDelRoutePointFunction()));
    moveRoutePointAction = new QAction(tr("Move route point"), this);
    moveRoutePointAction->setCheckable(true);
    functionActionGroup->addAction(moveRoutePointAction);
    connect(moveRoutePointAction, SIGNAL(triggered()), view, SLOT(setMoveRoutePointFunction()));
    editRoutePointAction = new QAction(tr("Edit route point"), functionActionGroup);
    editRoutePointAction->setCheckable(true);
    connect(editRoutePointAction, SIGNAL(triggered()), view, SLOT(setEditRoutePointFunction()));
    insertRoutePointAction = new QAction(tr("Insert route point"), functionActionGroup);
    insertRoutePointAction->setCheckable(true);
    connect(insertRoutePointAction, SIGNAL(triggered()), view, SLOT(setInsertRoutePointFunction()));
    saveRouteAction = new QAction(tr("Save Route ..."), this);
    connect(saveRouteAction, SIGNAL(triggered()), this, SLOT(saveRoute()));
    delRouteAction = new QAction(tr("Delete complete route"), this);
    connect(delRouteAction, SIGNAL(triggered()), this, SLOT(delRoute()));
    saveRouteProfileAction = new QAction(tr("Save route profile..."), this);
    connect(saveRouteProfileAction, SIGNAL(triggered()), this, SLOT(saveRouteProfile()));
    routeAddSrtmEleAction = new QAction(tr("Add SRTM elevation"), this);
    connect(routeAddSrtmEleAction, SIGNAL(triggered()), this, SLOT(routeAddSrtmEle()));
    openPhotoAction = new QAction(tr("Open photos..."), this);
    connect(openPhotoAction, SIGNAL(triggered()), this, SLOT(openPhotos()));
    showPhotoAction = new QAction(tr("Show photo list"), this);
    connect(showPhotoAction, SIGNAL(triggered()), this, SLOT(showPhotoWidget()));
    showPhotoAction->setEnabled(false);
    fixPhotoTimestampAction = new QAction(tr("Fix Photo Timestamps"), this);
    connect(fixPhotoTimestampAction, SIGNAL(triggered()), this, SLOT(fixPhotoTimestamp()));
    geoTagPhotoAction = new QAction(tr("Geotag all photos"), this);
    connect(geoTagPhotoAction, SIGNAL(triggered()), this, SLOT(geoTagPhotos()));
    //hidePhotoAction = new QAction(tr("Hide photos"), this);
    //connect(hidePhotoAction, SIGNAL(triggered()), this, SLOT(hidePhotos()));
    showTrackPoiAction = new QAction(tr("Track POIs"), this);
    connect(showTrackPoiAction, SIGNAL(triggered()), this, SLOT(showTrackPois()));
    showTrackProfileAction = new QAction(tr("Track Profile"), this);
    connect(showTrackProfileAction, SIGNAL(triggered()), this, SLOT(showTrackProfile()));
    typeActionGroup = new QActionGroup(this);
    typeActionGroup->setExclusive(true);
    ovlActionGroup = new QActionGroup(this);
    ovlActionGroup->setExclusive(false);
    showGridAction = new QAction(QIcon(":/icons/grid.png"), tr("Show grid"), this);
    showGridAction->setToolTip(tr("Show/hide distance grid"));
    showGridAction->setCheckable(true);
    connect(showGridAction, SIGNAL(triggered()), this, SLOT(toggleGrid()));
    showTileBoundAction = new QAction(tr("Tile bounds"), this);
    showTileBoundAction->setToolTip(tr("Show/hide tile bounds"));
    showTileBoundAction->setCheckable(true);
    connect(showTileBoundAction, SIGNAL(triggered()), this, SLOT(toggleTileBounds()));
    addNorthAction = new QAction(tr("Extend to north"), this);
    addNorthAction->setIcon(QIcon(":/icons/add.png"));
    addEastAction = new QAction(tr("Extend to east"), this);
    addEastAction->setIcon(QIcon(":/icons/add.png"));
    addSouthAction = new QAction(tr("Extend to south"), this);
    addSouthAction->setIcon(QIcon(":/icons/add.png"));
    addWestAction = new QAction(tr("Extend to west"), this);
    addWestAction->setIcon(QIcon(":/icons/add.png"));
    delNorthAction = new QAction(tr("Remove from north"), this);
    delNorthAction->setIcon(QIcon(":/icons/delete.png"));
    delEastAction = new QAction(tr("Remove from east"), this);
    delEastAction->setIcon((QIcon(":/icons/delete.png")));
    delSouthAction = new QAction(tr("Remove from south"), this);
    delSouthAction->setIcon((QIcon(":/icons/delete.png")));
    delWestAction = new QAction(tr("Remove from west"), this);
    delWestAction->setIcon(QIcon(":/icons/delete.png"));
    moveNorthAction = new QAction(tr("Move to north"), this);
    moveNorthAction->setIcon(QIcon(":/icons/arrow_up.png"));
    moveEastAction = new QAction(tr("Move to east"), this);
    moveEastAction->setIcon(QIcon(":/icons/arrow_right.png"));
    moveSouthAction = new QAction(tr("Move to south"), this);
    moveSouthAction->setIcon(QIcon(":/icons/arrow_down.png"));
    moveWestAction = new QAction(tr("Move to west"), this);
    moveWestAction->setIcon(QIcon(":/icons/arrow_left.png"));
    setCenterAction = new QAction(tr("Set map center"), this);
    searchPlaceAction = new QAction(tr("Search place..."), this);
    connect(searchPlaceAction, SIGNAL(triggered()), this, SLOT(searchPlace()));
    editLayersAction = new QAction(tr("Edit base layers..."), this);
    editOverlaysAction = new QAction(tr("Edit overlays..."), this);
    editSettingsAction = new QAction(tr("Edit settings..."), this);
    helpAction = new QAction(tr("Help"), this);
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));

    connect(deleteTrackAction, SIGNAL(triggered()), this, SLOT(deleteTrack()));
    connect(lastTrackPosAction, SIGNAL(triggered()), this, SLOT(lastTrackPos()));
    connect(redrawAction, SIGNAL(triggered()), scene, SLOT(redraw()));
    connect(addNorthAction, SIGNAL(triggered()), this, SLOT(addNorth()));
    connect(addEastAction, SIGNAL(triggered()), this, SLOT(addEast()));
    connect(addSouthAction, SIGNAL(triggered()), this, SLOT(addSouth()));
    connect(addWestAction, SIGNAL(triggered()), this, SLOT(addWest()));
    connect(delNorthAction, SIGNAL(triggered()), this, SLOT(delNorth()));
    connect(delEastAction, SIGNAL(triggered()), this, SLOT(delEast()));
    connect(delSouthAction, SIGNAL(triggered()), this, SLOT(delSouth()));
    connect(delWestAction, SIGNAL(triggered()), this, SLOT(delWest()));
    connect(moveNorthAction, SIGNAL(triggered()), this, SLOT(moveNorth()));
    connect(moveEastAction, SIGNAL(triggered()), this, SLOT(moveEast()));
    connect(moveSouthAction, SIGNAL(triggered()), this, SLOT(moveSouth()));
    connect(moveWestAction, SIGNAL(triggered()), this, SLOT(moveWest()));
    connect(setCenterAction, SIGNAL(triggered()), this, SLOT(setCenter()));
    connect(editLayersAction, SIGNAL(triggered()), this, SLOT(editBaseLayers()));
    connect(editOverlaysAction, SIGNAL(triggered()), this, SLOT(editOverlays()));
    connect(editSettingsAction, SIGNAL(triggered()), this, SLOT(editSettings()));
}

void MainWindow::enableTrackActions(bool enable) {
    saveTrackAction->setEnabled(enable);
    saveTrackProfileAction->setEnabled(enable);
    moveTrackPosAction->setEnabled(enable);
    firstTrackPosAction->setEnabled(enable);
    decTrackPosAction->setEnabled(enable);
    dDecTrackPosAction->setEnabled(enable);
    incTrackPosAction->setEnabled(enable);
    dIncTrackPosAction->setEnabled(enable);
    lastTrackPosAction->setEnabled(enable);
    deleteTrackAction->setEnabled(enable);
    trackBoundingBoxAction->setEnabled(enable);
    trackSimplifyAction->setEnabled(enable);
    editTrackPointAction->setEnabled(enable);
    deleteTrackPosAction->setEnabled(enable);
    delTrackPartAction->setEnabled(enable);
    showTrackPoiAction->setEnabled(enable);
    showTrackProfileAction->setEnabled(enable);
    lTrackPos->setVisible(true);
}

void MainWindow::enablePhotoActions(bool enable) {
    fixPhotoTimestampAction->setEnabled(enable);
    geoTagPhotoAction->setEnabled(enable);
}

void MainWindow::createBaselayerActions() {
    foreach (QAction *a, typeActionGroup->actions()) {
        typeActionGroup->removeAction(a);
        delete a;
    }
    QList<Layer> baseLayers = settings.baseLayers();
    for (int i = 0; i < baseLayers.size(); i++) {
        QAction *a = new QAction(baseLayers[i].name(), this);
        a->setData(i);
        a->setCheckable(true);
        if (i == 0)
            a->setChecked(true);
        connect(a, SIGNAL(triggered()), this, SLOT(changeBaseLayer()));
        typeActionGroup->addAction(a);
        mType->addAction(a);
    }
}

void MainWindow::createOverlayActions() {
    foreach (QAction *a, ovlActionGroup->actions()) {
        ovlActionGroup->removeAction(a);
        delete a;
    }
    QList<Layer> overlays = settings.overlays();
    for (int i = 0; i < overlays.size(); i++) {
        QAction *a = new QAction(overlays[i].name(), this);
        a->setData(i);
        a->setCheckable(true);
        connect(a, SIGNAL(triggered()), this, SLOT(changeOverlays()));
        ovlActionGroup->addAction(a);
        mOverlays->addAction(a);
    }
    foreach (QAction *a, ovlActionGroup->actions()) {
        toolBar->addAction(a);
    }
}

void MainWindow::createMenuBar() {
    QMenu *mFile = menuBar()->addMenu(tr("&File"));
    mFile->addAction(loadGpxAction);
    mFile->addAction(printAction);
    mFile->addAction(savePixmapAction);
    mFile->addAction(setCenterAction);
    mFile->addAction(searchPlaceAction);
    mFile->addAction((quitAction));
    QMenu *mTrack = menuBar()->addMenu(tr("&Track"));
    mTrack->addAction(trackFromGpsAction);
    mTrack->addAction(loadTrackAction);
    mTrack->addAction(saveTrackAction);
    mTrack->addAction(saveTrackProfileAction);
    mTrack->addAction(trackSimplifyAction);
    mTrack->addSeparator();
    mTrack->addAction(moveTrackPosAction);
    mTrack->addAction(deleteTrackAction);
    mTrack->addAction(firstTrackPosAction);
    mTrack->addAction(decTrackPosAction);
    mTrack->addAction(dDecTrackPosAction);
    mTrack->addAction(incTrackPosAction);
    mTrack->addAction(dIncTrackPosAction);
    mTrack->addAction(lastTrackPosAction);
    mTrack->addAction(trackBoundingBoxAction);
    mTrack->addAction(delTrackPartAction);
    mTrack->addAction(editTrackPointAction);
    mTrack->addAction(deleteTrackPosAction);
    QMenu *mGpx = menuBar()->addMenu(tr("&Route/Waypoint"));
    mGpx->addAction(newWaypointAction);
    mGpx->addAction(newRoutePointAction);
    mGpx->addAction(moveGpxPointAction);
    mGpx->addAction(delRoutePointAction);
    mGpx->addAction(editRoutePointAction);
    mGpx->addAction(insertRoutePointAction);
    mGpx->addAction(saveRouteAction);
    mGpx->addAction(routeAddSrtmEleAction);
    mGpx->addAction(saveRouteProfileAction);
    mGpx->addAction(delRouteAction);
    QMenu *mView = menuBar()->addMenu(tr("&View"));
    mView->addAction(redrawAction);
    mView->addAction(showGridAction);
    mView->addAction(showTileBoundAction);
    mType = mView->addMenu(tr("&Type"));
    foreach (QAction *a, typeActionGroup->actions()) {
        mType->addAction(a);
    }
    mView->addAction(showTrackPoiAction);
    mView->addAction(showTrackProfileAction);
    mOverlays = mView->addMenu(tr("&Overlays"));
    foreach (QAction *a, ovlActionGroup->actions()) {
        mOverlays->addAction(a);
    }
    QMenu *mPhotos = menuBar()->addMenu(tr("&Photos"));
    mPhotos->addAction(openPhotoAction);
    mPhotos->addAction(showPhotoAction);
    mPhotos->addAction(fixPhotoTimestampAction);
    mPhotos->addAction(geoTagPhotoAction);
    //mPhotos->addAction(hidePhotoAction);

    QMenu *mSettings = menuBar()->addMenu(tr("&Settings"));
    mSettings->addAction(editLayersAction);
    mSettings->addAction(editOverlaysAction);
    mSettings->addAction(editSettingsAction);

    QMenu *mHelp = menuBar()->addMenu(tr("&?"));
    mHelp->addAction(helpAction);
}

void MainWindow::createToolBar() {
    toolBar = addToolBar(tr("Tools"));
    toolBar->addAction(printAction);
    toolBar->addSeparator();
    toolBar->addAction(redrawAction);
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
    toolBar->addAction(showGridAction);
    toolBar->addSeparator();
    trackToolBar = addToolBar(tr("Track"));
    trackToolBar->addAction(firstTrackPosAction);
    trackToolBar->addAction(dDecTrackPosAction);
    trackToolBar->addAction(decTrackPosAction);
    trackToolBar->addAction(editTrackPointAction);
    trackToolBar->addAction(incTrackPosAction);
    trackToolBar->addAction(dIncTrackPosAction);
    trackToolBar->addAction(lastTrackPosAction);
    trackToolBar->setVisible(false);
}

void MainWindow::createStatusBar() {
    lModelStatus = new QLabel();
    lModelStatus->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statusBar()->addWidget(lModelStatus);
    lPos = new QLabel();
    lPos->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statusBar()->addWidget(lPos);
    lTrackPos = new QLabel("");
    lTrackPos->setMinimumWidth(250);
    lTrackPos->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statusBar()->addWidget(lTrackPos);
    lPhotoDir = new QLabel("");
    statusBar()->addWidget(lPhotoDir);
}

void MainWindow::createProfileWidget() {
    profileWidget = new QDockWidget(tr("Profile"));
    QWidget *content = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *control = new QVBoxLayout();
    QRadioButton *bEle = new QRadioButton(tr("GPX Data"), profileWidget);
    control->addWidget(bEle);
    QRadioButton *bSrtm = new QRadioButton(tr("SRTM Data"), profileWidget);
    control->addWidget(bSrtm);
    control->addStretch();
    layout->addLayout(control);
    profileScene = new ProfileScene(model, &settings);
    profileView = new ProfileView(profileScene);
    layout->addWidget(profileView);
    content->setLayout(layout);
    profileWidget->setWidget(content);
    profileWidget->hide();
    connect(bEle, SIGNAL(clicked()), profileScene, SLOT(setEle()));
    connect(bSrtm, SIGNAL(clicked()), profileScene, SLOT(setSrtm()));
    bEle->click();
}

void MainWindow::createTrackPoiTable() {
    QVBoxLayout *control = new QVBoxLayout();
    trackPoiWidget->setLayout(control);
    trackPoiListView = new QTableView();
    trackPoiListView->setModel(myTrackPoiModel);
    trackPoiListView->setItemDelegate(new GpxListDelegate());
    trackPoiListView->verticalHeader()->hide();
    QHeaderView *headerView = new QHeaderView(Qt::Horizontal, trackPoiListView);
    trackPoiListView->setHorizontalHeader(headerView);
    headerView->setDefaultAlignment(Qt::AlignLeft);
    headerView->setResizeMode(0, QHeaderView::Stretch);
    headerView->setResizeMode(1, QHeaderView::Fixed);
    headerView->resizeSection(1, 45);
    headerView->setResizeMode(2, QHeaderView::Fixed);
    headerView->resizeSection(2, 45);
    trackPoiWidget->setWidget(trackPoiListView);
    trackPoiWidget->hide();
}

void MainWindow::showImage(HttpGet *getter) {
    qDebug()<<"showImage";
    QPixmap pixmap;
    bool res = pixmap.loadFromData(getter->getData());
    qDebug()<<"pixmap "<<res;
    if (res) {
        //label->setPixmap(pixmap);
        delete getter;
        getter = 0;
    }
}

void MainWindow::print() {
    QPrintDialog dlg(printer, this);
    if (dlg.exec()) {
        output(printer);
    }
}

void MainWindow::paintTiles(QPainter *painter, bool showOverlays) {
    int w = model->width();
    int h = model->height();
    Layer layer = model->layer();
    QList<Layer> *overlays = model->overlays();
    for (int ix = 0; ix < w; ix++) {
        for (int iy = 0; iy < h; iy++) {
            QString key = layer.url();
            key = key.replace(QString("$z"), QString::number(model->zoom()))
                     .replace(QString("$y"), QString::number(model->y()+iy))
                     .replace(QString("$x"), QString::number(model->x()+ix));
            qDebug()<<"key: "<<key;
            QPixmap *px = model->getPixmap(key);
            if (px == NULL) {
                qDebug()<<"key "<<key<<" not found";
                continue;
            }
            painter->drawPixmap(QRect(ix*256, iy*256, 256, 256), *px);
            if (!showOverlays)
                continue;
            foreach(Layer ly, *overlays) {
                key = ly.url();
                key = key.replace(QString("$z"), QString::number(model->zoom()))
                         .replace(QString("$y"), QString::number(model->y()+iy))
                         .replace(QString("$x"), QString::number(model->x()+ix));
                qDebug()<<"key: "<<key;
                QPixmap *px = model->getPixmap(key);
                if (px == NULL) {
                    qDebug()<<"key "<<key<<" not found";
                    continue;
                }
                painter->drawPixmap(QRect(ix*256, iy*256, 256, 256), *px);
            }
        }
    }
}

void MainWindow::paintTrack(QPainter *painter, bool showSym) {
    QPen pen(settings.outTrackColor());
    pen.setWidth(settings.outTrackWidth());
    painter->setPen(pen);
    QPainterPath path;
    bool start = true;
    foreach(const GpxPoint& point, model->track().trackPoints()) {
        QPoint p = model->lonLat2Scene(point.coord());
        if (start) {
            path.moveTo(p);
            start = false;
        } else {
            path.lineTo(p);
        }
    }
    painter->drawPath(path);
    if (!showSym) return;
    foreach(const GpxPoint& point, model->track().trackPoints()) {
        if (point.sym() == "") continue;
        QPoint p = model->lonLat2Scene(point.coord());
        MapIcon ico = settings.mapIconList().icon(point.sym());
        QPixmap px(ico.mapIcoFile());
        int offx = px.width()/2;
        int offy = px.height()/2;
        painter->drawPixmap(QPoint(p.x()-offx, p.y()-offy), px);
    }
}

void MainWindow::paintRoute(QPainter *painter, bool showSym) {
    QPen pen(settings.outRouteColor());
    pen.setWidth(settings.outRouteWidth());
    painter->setPen(pen);
    QPainterPath path;
    bool start = true;
    foreach(const GpxPoint& point, *model->route().points()) {
        QPoint p = model->lonLat2Scene(point.coord());
        if (start) {
            path.moveTo(p);
            start = false;
        } else {
            path.lineTo(p);
        }
    }
    painter->drawPath(path);
    if (!showSym) return;
    foreach(const GpxPoint& point, *model->route().points()) {
        if (point.sym() == "") continue;
        QPoint p = model->lonLat2Scene(point.coord());
        MapIcon ico = settings.mapIconList().icon(point.sym());
        QPixmap px(ico.mapIcoFile());
        int offx = px.width()/2;
        int offy = px.height()/2;
        painter->drawPixmap(QPoint(p.x()-offx, p.y()-offy), px);
    }
}

void MainWindow::paintWpt(QPainter *painter) {
    foreach(const GpxPoint& point, model->waypoints()) {
        if (point.sym() == "") continue;
        QPoint p = model->lonLat2Scene(point.coord());
        MapIcon ico = settings.mapIconList().icon(point.sym());
        QPixmap px(ico.mapIcoFile());
        int offx = px.width()/2;
        int offy = px.height()/2;
        painter->drawPixmap(QPoint(p.x()-offx, p.y()-offy), px);
        painter->setPen(Qt::black);
        painter->drawText(p.x()+offx, p.y(), point.name());
    }
}

void MainWindow::paintGrid(QPainter *painter) {
    QPointF lonLat0 = model->lonLat(QPointF(0, 0));
    QPointF lonLat1 = model->lonLat(QPointF(0, model->height()*256));
    double distKm = Model::geodist1(lonLat0, lonLat1);
    double lineDist = model->height()*256/distKm;
    while (lineDist < 20)
        lineDist *= 10;
    QPen pen(Qt::gray);
    pen.setWidth(1);
    painter->setPen(pen);
    for (double x = 0; x < model->width()*256; x += lineDist) {
        painter->drawLine(x, 0, x, model->height()*256);
    }
    for (double y = 0; y < model->height()*256; y += lineDist) {
        painter->drawLine(0, y, model->width()*256, y);
    }
}

void MainWindow::paintCopy(QPainter *painter, int w, int h, const QString& copy) {
    painter->setPen(QColor(128, 128, 128));
    painter->setFont(QFont("Helvetica", 8));
    painter->drawText(QRect(0, h-30, w-10, 20), Qt::AlignRight|Qt::AlignBottom, copy);
}

QPixmap* MainWindow::createPixmap() {
    OutputSelDlg dlg;
    dlg.setGrid(scene->isShowGrid());
    dlg.setTrack(scene->isShowTrack());
    dlg.setTrackSym(scene->isShowTrack());
    dlg.setRoute(model->route().points()->size() > 0);
    dlg.setRouteSym(dlg.getRoute());
    dlg.setWptSym(model->waypoints().size() > 0);
    if (dlg.exec() != QDialog::Accepted) {
        return 0;
    }
    bool showOverlays = dlg.getOverlays();
    bool showGrid = dlg.getGrid();
    bool showTrack = dlg.getTrack();
    bool showTrackSym = dlg.getTrackSym();
    bool restrict = dlg.getRestrict();
    bool showRoute = dlg.getRoute();
    bool showRouteSym = dlg.getRouteSym();
    bool showWptSym = dlg.getWptSym();
    QString copy = dlg.copy();
    int w = model->width()*256;
    int h = model->height()*256;
    int x0 = 0;
    int y0 = 0;
    if (!model->track().isEmpty() && restrict) {
        BoundingBox bb = model->track().boundingBox();
        QPoint p0 = model->lonLat2Scene(bb.p0())+QPoint(-10, 10);
        QPoint p1 = model->lonLat2Scene(bb.p1())+QPoint(10, -10);
        if (p0.x() < 0) p0.setX(0);
        if (p0.y() < 0) p0.setY(0);
        if (p1.x() > w) p1.setX(w);
        if (p1.y() > h) p1.setY(h);
        qDebug()<<"BoundingBox"<<p0<<" "<<p1;
        x0 = p0.x();
        y0 = p1.y();
        w = p1.x()-p0.x();
        h = p0.y()-p1.y();
    }
    QPixmap *pixmap = new QPixmap(w, h);
    QPainter painter(pixmap);
    painter.translate(-x0, -y0);
    paintTiles(&painter, showOverlays);
    if (showGrid) {
        paintGrid(&painter);
    }
    if (showTrack && !model->track().isEmpty()) {
        paintTrack(&painter, showTrackSym);
    }
    if (showRoute && !model->route().isEmpty()) {
        paintRoute(&painter, showRouteSym);
    }
    if (showWptSym) paintWpt(&painter);
    paintCopy(&painter, w, h, copy);
    return pixmap;
}

void MainWindow::savePixmap() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save image"),
      QString(), tr("Image files (*.png *.jpg)"));
    if (filename.isEmpty())
        return;
    QPixmap *pixmap = createPixmap();
    if (pixmap == 0)
        return;
    //scene->render(&painter, QRectF(0, 0, w, h), QRectF(x0, y0, w, h));
    pixmap->save(filename);
    delete pixmap;
}

void MainWindow::output(QPrinter *device) {
    QPixmap *pixmap = createPixmap();
    if (pixmap == 0)
        return;
    int w = pixmap->width();
    int h = pixmap->height();
    double tilesize = settings.tileSize(); // mm
    qDebug()<<device->pageRect(QPrinter::Millimeter)<<" "<<device->pageRect();
    int dpix = device->logicalDpiX();
    int dpiy = device->logicalDpiY();
    qDebug()<<"width: "<<device->width()<<" "<<dpix;
    double tw = tilesize/25.4*dpix/256;
    double th = tilesize/25.4*dpiy/256;
    int maxw = (int)(device->width()/tw);
    int maxh = (int)(device->height()/th);
    int pagesx = w/maxw +1;
    int pagesy = h/maxh+1;
    int dw = pagesx == 1 ? w : w/pagesx+1;
    int dh = pagesy == 1 ? h : h/pagesy+1;
    qDebug()<<"pagesx: "<<pagesx<<" pagesy: "<<pagesy;
    if (QMessageBox::question(this,
            tr("Map print information"),
            tr("Tile size: %1 mm\n%2x%3 pages.").arg(tilesize).arg(pagesx).arg(pagesy),
            QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok) != QMessageBox::Ok) {
        delete pixmap;
        return;
    }
    QPainter painter(device);
    QRectF target(0, 0, dw*tw, dh*th);
    for (int ix = 0; ix < pagesx; ix++) {
        for (int iy = 0; iy < pagesy; iy++) {
            qDebug()<<"Page "<<ix<<","<<iy;
            if (ix+iy > 0) {
                device->newPage();
            }
            painter.drawPixmap(target, *pixmap, QRectF(ix*dw, iy*dh, dw, dh));
            painter.drawText(QRectF(0, device->height()-0.2*dpiy, 5*dpix, 0.2*dpiy),
                                 tr("Data by www.openstreetmap.org"));
        }
    }
    delete pixmap;
    painter.drawText(QRectF(0, device->height()-0.2*dpiy, 5*dpix, 0.2*dpiy),
                     tr("Data by www.openstreetmap.org"));
    painter.end();
}

void MainWindow::changeBaseLayer() {
    QAction *a = typeActionGroup->checkedAction();
    if (a != NULL) {
        int idx = a->data().toInt();
        Layer baseLayer = settings.baseLayers()[idx];
        qDebug()<<"New Layer: "<<baseLayer.name();
        model->setLayer(baseLayer);
    }
}

void MainWindow::changeOverlays() {
    QList<Layer> ovls;
    foreach (QAction *a, ovlActionGroup->actions()) {
        if (a->isChecked()) {
            int idx = a->data().toInt();
            ovls.append(settings.overlays()[idx]);
        }
    }
    model->changeOverlays(ovls);
}

void MainWindow::toggleGrid() {
    scene->setShowGrid(showGridAction->isChecked());
}

void MainWindow::toggleTileBounds() {
    scene->setShowTileBounds(showTileBoundAction->isChecked());
}

void MainWindow::addNorth() {
    model->changeSize(1, 0, 0, 0);
}

void MainWindow::addEast() {
    model->changeSize(0, 1, 0, 0);
}

void MainWindow::addSouth() {
    model->changeSize(0, 0, 1, 0);
}

void MainWindow::addWest() {
    model->changeSize(0, 0, 0, 1);
}

void MainWindow::delNorth() {
    model->changeSize(-1, 0, 0, 0);
}

void MainWindow::delEast() {
    model->changeSize(0, -1, 0, 0);
}

void MainWindow::delSouth() {
    model->changeSize(0, 0, -1, 0);
}

void MainWindow::delWest() {
    model->changeSize(0, 0, 0, -1);
}

void MainWindow::moveNorth() {
    model->changeSize(1, 0, -1, 0);
}

void MainWindow::moveEast() {
    model->changeSize(0, 1, 0, -1);
}

void MainWindow::moveSouth() {
    model->changeSize(-1, 0, 1, 0);
}

void MainWindow::moveWest() {
    model->changeSize(0, -1, 0, 1);
}

void MainWindow::setCenter() {
    QPoint pos(model->width()*128, model->height()*128);
    QPointF lonLat = model->lonLat(pos);
    qDebug()<<"Map center: "<<lonLat;
    CenterDialog dlg(lonLat, scene);
    if (dlg.exec()) {
        model->setCenter(dlg.lonLat());
        if (dlg.saveVals()) {
            settings.setCenter(dlg.lonLat());
            settings.setZoom(model->zoom());
            settings.setXExt(model->width());
            settings.setYExt(model->height());
            settings.save(this);
        }
    }
}

void MainWindow::searchPlace() {
    PlaceDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        model->setCenter(dlg.lonLat());
    }
}

void MainWindow::updateModelStatus() {
    lModelStatus->setText(tr("width: %1 height: %2 zoom: %3")
                          .arg(model->width()).arg(model->height()).arg(model->zoom()));
}

void MainWindow::showGeoPos(const QPointF &pos) {
    lPos->setText(tr("(%1, %2)").arg(pos.x(), 0, 'f', 5).arg(pos.y(), 0, 'f', 5));
}

void MainWindow::editBaseLayers() {
    LayersDialog dlg(settings.baseLayers());
    if (dlg.exec() == QDialog::Accepted) {
        qDebug()<<dlg.layers().size();
        settings.setBaseLayers(dlg.layers());
        settings.save(this);
        createBaselayerActions();
        model->setLayer(settings.baseLayers()[0]);
    }
}

void MainWindow::editOverlays() {
    LayersDialog dlg(settings.overlays());
    if (dlg.exec() == QDialog::Accepted) {
        qDebug()<<dlg.layers().size();
        settings.setOverlays(dlg.layers());
        settings.save(this);
        createOverlayActions();
        //model->setLayer(&baseLayers[0]);
    }

}

void MainWindow::editSettings() {
    SettingsDialog dlg(settings, this);
    if (dlg.exec() == QDialog::Accepted) {
        settings = dlg.settings();
        settings.save(this);
        model->updateSettings(settings);
    }
}

void MainWindow::readTrackFromGps() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Read GPS device"), settings.trackDir(),
                                                    tr("GPX files (*.gpx)"));
    if (filename.isEmpty()) return;
    int idx = filename.lastIndexOf(".");
    QString rawFileName;
    if (idx < 0) {
        rawFileName = filename+"-raw.gpx";
    }
    else {
        rawFileName = filename.left(idx)+"-raw.gpx";
    }
    QStringList params;
    params<<"-w"<<"-t"
          <<"-i"<<settings.gpsDevice()
          <<"-f"<<settings.gpsInterface()
          <<"-o"<<"gpx"<<"-F"<<rawFileName;
    qDebug()<<settings.gpsbabel()<<params;
    QProcess gpsbabel;
    gpsbabel.start(settings.gpsbabel(), params);
    if (!gpsbabel.waitForStarted()) {
        QMessageBox::warning(this, tr("Cannot download track"), tr("Cannot start gpsbabel"));
        return;
    }
    if (!gpsbabel.waitForFinished()) {
        QMessageBox::warning(this, tr("Cannot download track"), tr("Cannot download track %1: %2").arg(rawFileName)
                             .arg(gpsbabel.errorString()));
        return;
    }
    Gpx gpx(rawFileName);
    GpxPointList ptl = selectTrackSegments(gpx);
    if (ptl.size() > 0) {
        model->trackSetNew(filename, gpx.trackName(), ptl);
        enableTrackActions(true);
        profileView->setVisible(true);
        trackToolBar->setVisible(true);
        model->setTrackPos(0);
    }
    if (gpx.wayPoints().size() > 0) {
        model->waypointsSetNew(gpx.wayPoints());
    }
    view->centerView();
}

void MainWindow::loadGpx() {
    QString dir = settings.trackDir();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load GPX file"), dir,
                                               tr("GPX file (*.gpx)"));
    if (fileName.isEmpty())
        return;
    Gpx gpx(fileName);
    if (gpx.trackSegments().size() > 0) {
        GpxPointList ptl = selectTrackSegments(gpx);
        if (ptl.size() > 0) {
            model->trackSetNew(fileName, gpx.trackName(), ptl);
            // TODO: SIGNAL->SLOT
            qDebug()<<"updatePointList track";
            myTrackPoiModel->updatePointList();
            if (myTrackPoiModel->rowCount(QModelIndex()) > 0) showTrackPois();
            enableTrackActions(true);
            profileView->setVisible(true);
            trackToolBar->setVisible(true);
            model->setTrackPos(0);
            profileWidget->show();
        }
    }
    if (gpx.wayPoints().size() > 0) {
        model->waypointsSetNew(gpx.wayPoints());
    }
    if (gpx.routePoints().size() > 0) {
        model->routeSetNew(fileName, gpx.routeName(), gpx.routePoints());
    }
    view->centerView();
}

void MainWindow::saveTrack() {
    SaveRouteDlg dlg(SaveRouteDlg::TRACK, model->track().fileName(), model->track().name());
    dlg.setWaypoints(model->waypoints().size() > 0);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    QString fileName = dlg.fileName();
    QString name = dlg.name();
    bool isUpload = dlg.isUpload();
    model->trackPtr()->setName(name);
    QFile file(fileName);
    file.open(QFile::WriteOnly|QFile::Text);
    model->saveModifiedTrack(&file, dlg.isWaywaypoints(), false);
    file.close();
    model->trackPtr()->setFileName(fileName);
    if (isUpload) {
        QStringList params;
        params<<"-t"
              <<"-i"<<"gpx"<<"-f"<<fileName
              <<"-o"<<settings.gpsDevice()
              <<"-F"<<settings.gpsInterface();
        qDebug()<<settings.gpsbabel()<<params;
        run(settings.gpsbabel(), params);
    }
}

GpxPointList MainWindow::selectTrackSegments(const Gpx& gpx) {
    if (gpx.trackSegments().size() == 1) return gpx.trackSegments().at(0);
    QList<TrackSegInfo> segInfo;
    for (int i = 0; i < gpx.trackSegments().size(); i++) {
        segInfo.append(gpx.trackSegInfo(i));
    }
    TrackSelDialog dlg(segInfo);
    GpxPointList res;
    if (dlg.exec()) {
        foreach(int idx, dlg.selectedRows()) {
            qDebug()<<"selected: "<<idx;
            foreach (const GpxPoint& p, gpx.trackSegments().at(idx)) {
                res.append(p);
            }
        }
    }
    return res;
}

void MainWindow::deleteTrack() {
    if (model->track().isEmpty())
        return;
    if (QMessageBox::question(this, tr("Delete track"), tr("Delete current track?"),
                             QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::Yes) {
        model->delTrack();
        trackToolBar->setVisible(false);
        profileWidget->hide();
        enableTrackActions(false);
    }
}

void MainWindow::incTrackPos() {
    model->changeTrackPos(1);
}

void MainWindow::dIncTrackPos() {
    model->changeTrackPos(10);
}

void MainWindow::decTrackPos() {
    model->changeTrackPos(-1);
}

void MainWindow::dDecTrackPos() {
    model->changeTrackPos(-10);
}

void MainWindow::firstTrackPos() {
    model->setTrackPos(0);
}

void MainWindow::lastTrackPos() {
    model->setTrackPos(model->track().trackPoints().size()-1);
}

void MainWindow::changeTrackPos(int pos) {
    if (model->track().isEmpty())
        return;
    GpxPoint p = model->track().trackPoint(pos);
    qDebug()<<"changed pos "<<p.coord();
    lTrackPos->setText(QString("%1 %2 (%3, %4) %5 mNN, %6 km")
                       .arg(pos)
                       .arg(p.timestamp().toLocalTime().toString("hh:mm"))
                       .arg(p.coord().x(), 0, 'f', 5)
                       .arg(p.coord().y(), 0, 'f', 5)
                       .arg(p.ele(), 0, 'f', 0)
                       .arg(model->track().sumDist(), 0, 'f', 1));
}

void MainWindow::toggleTrackBoundingBox() {
    scene->setShowTrackBb(trackBoundingBoxAction->isChecked());
}

void MainWindow::simplifyTrack() {
    model->uniqueTrack();
    TrackSimplifyDlg dlg(scene);
    if (dlg.exec() != QDialog::Accepted) return;
    if (dlg.action() == TrackSimplifyDlg::REPLACE) {
        model->trackSetNew(dlg.fileName(), model->track().name()+" (S)", dlg.simpleTrack()->trackPoints());
    } else if (dlg.action() == TrackSimplifyDlg::EXPORT) {
        QFileInfo fi(dlg.fileName());
        TrackExportDlg expdlg(settings.exportDir()+"/"+fi.fileName(), this);
        if (model->waypoints().size() > 0) expdlg.setWpts(true);
        if (expdlg.exec() == QDialog::Accepted) {
            QFile file(expdlg.fileName());
            if (!file.open(QFile::WriteOnly|QFile::Text)) return;
            if (expdlg.hasWpts()) dlg.simpleTrack()->writeModifiedXml(&file, model->waypoints(), expdlg.isSimple());
            else                  dlg.simpleTrack()->writeModifiedXml(&file, GpxPointList(), expdlg.isSimple());
            file.close();
            if (expdlg.isOsm()) {
                QPointF center = dlg.simpleTrack()->boundingBox().center();
                OsmMap map(center, model->zoom());
                map.writeTrackFile(expdlg.osmFileName(), expdlg.fileName(), expdlg.title(), settings.mapIconList());
            }
        }
    }
}

void MainWindow::editTrackPoint() {
    if (model->track().isEmpty())
        return;
    int pos = model->track().pos();
    view->editTrackPoint(pos);
}

void MainWindow::deleteTrackPos() {
    int pos = model->track().pos();
    GpxPoint p = model->track().trackPoint(pos);
    if (QMessageBox::question(this, tr("Delete track point"),
                              tr("Delete the track point (%1,%2) %3")
                                .arg(p.coord().x()).arg(p.coord().y()).arg(p.timestamp().toString()),
                                QMessageBox::Ok|QMessageBox::Cancel)
        == QMessageBox::Ok) {
        model->delTrackPoint(pos);
    }
}

void MainWindow::saveTrackProfile() {
    QFileInfo fileInfo(model->track().fileName());
    QFileInfo newFile(fileInfo.baseName(), fileInfo.baseName()+"-profile.png");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Track Profile"), newFile.absoluteFilePath(), tr("PNG (*.png *.jpg);;PDF (*.pdf)"));
    if (fileName.isEmpty()) return;
    GpxProfileDlg dlg(model->track().trackPoints(), &settings);
    if (dlg.exec() != QDialog::Accepted) return;
    dlg.pixmap()->save(fileName);
}

void MainWindow::delRoute() {
    int routeSize = model->routePtr()->points()->size();
    if (QMessageBox::question(this, tr("Delete route"),
                              tr("Delete this route (%1 points)?").arg(routeSize),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        model->routePtr()->delRoute();
    }
}

void MainWindow::saveRoute() {
    SaveRouteDlg dlg(SaveRouteDlg::ROUTE, model->route().fileName(), model->route().name());
    dlg.setWaypoints(model->waypoints().size() > 0);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    QString filename = dlg.fileName();
    QString name = dlg.name();
    bool isUpload = dlg.isUpload();
    QFile file(filename);
    if (!file.open(QFile::WriteOnly|QFile::Text)) {
        QMessageBox::warning(this, tr("Save Route %1").arg(name),
                             tr("Cannot save file %1: %2").arg(filename).arg(file.errorString()));
        return;
    }
    GpxPointList *wpts = 0;
    if (dlg.isWaywaypoints()) wpts = model->wptPtr();
    model->routePtr()->setName(name);
    qDebug()<<"waypoints: "<<dlg.isWaywaypoints()<<" ptr: "<<wpts;
    model->routePtr()->writeXml(&file, wpts);
    file.close();
    if (isUpload) {
        QStringList params;
        params<<"-r"
              <<"-i"<<"gpx"<<"-f"<<filename
              <<"-o"<<settings.gpsDevice()
              <<"-F"<<settings.gpsInterface();
        qDebug()<<settings.gpsbabel()<<params;
        run(settings.gpsbabel(), params);
    }
}

void MainWindow::routeAddSrtmEle() {
    model->routeAddSrtmEle();
}

void MainWindow::saveRouteProfile() {
    // TODO
    QFileInfo fileInfo(model->route().fileName());
    QFileInfo newFile(fileInfo.baseName(), fileInfo.baseName()+"-profile.png");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Route Profile"), newFile.absoluteFilePath(), tr("PNG (*.png);;PDF (*.pdf)"));
    if (fileName.isEmpty()) return;
    GpxProfileDlg dlg(*model->route().points(), &settings);
    if (dlg.exec() != QDialog::Accepted) return;
    dlg.pixmap()->save(fileName);
}

void MainWindow::run(const QString& path, const QStringList& params) {
    QProcess proc;
    proc.start(settings.gpsbabel(), params);
    if (!proc.waitForStarted()) {
        QMessageBox::warning(this, tr("Process failed"), tr("Cannot start %1").arg(path));
        return;
    }
    if (!proc.waitForFinished()) {
        QMessageBox::warning(this, tr("Process failed"), tr("Process %1 failed: %2").arg(path)
                             .arg(proc.errorString()));
        return;
    }

}

void MainWindow::enableShowPhoto(bool visible) {
    showPhotoAction->setEnabled(!visible);
}

void MainWindow::showPhotoWidget() {
    photoWidget->setVisible(true);
}

void MainWindow::openPhotos() {
    QString sdir = QFileDialog::getExistingDirectory(this, tr("Select photo directory"), myPhotoDir);
    if (sdir.isEmpty()) return;
    myPhotoDir = sdir;
    emit photoDirChanged(myPhotoDir);
    photoList->clear();
    QDir dir(sdir);
    QStringList filter;
    filter<<"*.jpg";
    QStringList files = dir.entryList(filter, QDir::Files, QDir::Time);
    QList<Photo> photos;
    foreach (const QString& basefilename, files) {
        QString filename = dir.absoluteFilePath(basefilename);
        qDebug()<<filename;
        Photo photo(filename);
        photos.push_back(photo);
    }
    if (photos.size() == 0) return;
    qSort(photos);
    foreach(Photo photo, photos) {
        QDateTime timestamp = photo.timestamp().addSecs(myPhotoOffset);
        QListWidgetItem *it = new QListWidgetItem(timestamp.toLocalTime().toString("hh:mm:ss ")+photo.baseFileName());
        qDebug()<<"Add to List: "<<photo.filename()<<" "<<photo.timestamp();
        it->setToolTip(photo.filename());
        it->setData(PHOTO_FILENAME, photo.filename());
        it->setData(PHOTO_ORIGTIME, photo.timestamp());
        it->setData(PHOTO_TIMESTAMP, timestamp);
        it->setData(PHOTO_COORD, QVariant());
        if (photo.hasCoord()) {
            it->setData(PHOTO_COORD, photo.coord());
            it->setIcon(QIcon(":/icons/world.png"));
            it->setToolTip(QString("%1 (geotagged)").arg(photo.filename()));
        }
        photoList->addItem(it);
    }
    photoWidget->setVisible(true);
    enablePhotoActions(true);
    photoList->setCurrentRow(0);
}

void MainWindow::showPhotoData(QListWidgetItem *item) {
    if (item == 0) return;
    QString filename = item->data(PHOTO_FILENAME).toString();
    QDateTime timestamp = item->data(PHOTO_TIMESTAMP).toDateTime();
    qDebug()<<filename<<" "<<timestamp;
    Photo photo(filename);
    QPixmap thumb = photo.pixmap();
    photoThumb->setPixmap(thumb);
    if (item->data(PHOTO_COORD).isValid()) {
        // TODO
        qDebug()<<"Photo coord: "<<item->data(PHOTO_COORD).toPointF();
    }
    //photoThumb->setText(timestamp.toString());
//    int idx = item->data(PHOTO_TRACKIDX).toInt();
//    if (idx >= 0) model->setTrackPos(idx);
}

void MainWindow::selectPhotoPos(QListWidgetItem *item) {
    if (item == 0) return;
    qDebug()<<"selectPhotoPos "<<item;
    showPhotoData(item);
    if (model->isEmptyTrack()) return;
    QDateTime timestamp = item->data(PHOTO_TIMESTAMP).toDateTime();
    int pos = model->track().nearest(timestamp);
    int diff = model->track().trackPoint(pos).timestamp().secsTo(timestamp);
    if (abs(diff) < 300) model->setTrackPos(pos);
}

void MainWindow::setPhotoOffset() {
    QListWidgetItem *it = photoList->currentItem();
    QDateTime timestamp = it->data(PHOTO_ORIGTIME).toDateTime();
    PhotoOffsetDlg dlg(model, timestamp, myPhotoOffset);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    myPhotoOffset = dlg.offset();
    lPhotoOffset->setText(QString("%1").arg(myPhotoOffset));
    int cnt = photoList->count();
    for (int i = 0; i < cnt; i++) {
        QListWidgetItem *it = photoList->item(i);
        QDateTime timestamp = it->data(PHOTO_ORIGTIME).toDateTime().addSecs(myPhotoOffset);
        it->setData(PHOTO_TIMESTAMP, timestamp);
        QString filename = it->data(PHOTO_FILENAME).toString();
        it->setText(timestamp.toLocalTime().toString("hh:mm:ss ")+QFileInfo(filename).baseName());
    }
    //connectPhotos();
}

void MainWindow::fixPhotoTimestamp() {
    int cnt = photoList->count();
    for (int i = 0; i < cnt; i++) {
        QListWidgetItem *it = photoList->item(i);
        QString fileName = it->data(PHOTO_FILENAME).toString();
        Photo photo(fileName);
        photo.fixTimestamp(myPhotoOffset);
        it->setData(PHOTO_ORIGTIME, photo.timestamp());
    }
    myPhotoOffset = 0;
    lPhotoOffset->setText(QString("%1").arg(myPhotoOffset));
}

void MainWindow::geoTagPhotos() {
    int cnt = photoList->count();
    for (int i = 0; i < cnt; i++) {
        QListWidgetItem *it = photoList->item(i);
        QDateTime timestamp = it->data(PHOTO_TIMESTAMP).toDateTime();
        int pos = model->track().nearest(timestamp);
        int diff = model->track().trackPoint(pos).timestamp().secsTo(timestamp);
        if (abs(diff) > 300) continue;
        const QPointF &coord = model->track().trackPoint(pos).coord();
        QString fileName = it->data(PHOTO_FILENAME).toString();
        Photo photo(fileName);
        qDebug()<<"photo coord: "<<coord;
        photo.setGeoPos(coord);
    }
}


void MainWindow::showPhotoDir(const QString &dir) {
    lPhotoDir->setText(dir);
}

void MainWindow::showTrackPois() {
    trackPoiWidget->show();
}

void MainWindow::showTrackProfile() {
    profileWidget->show();
}

void MainWindow::selectTrackPoi(const QModelIndex &index) {
    int trackPos = myTrackPoiModel->data(index, GpxListModel::GpxIndexRole).toInt();
    model->setTrackPos(trackPos);
}

void MainWindow::help() {
    QString lang = QLocale::system().name().left(2);
    qDebug()<<"Language: "<<lang;
    myHelpWin->setUrl(QString(":/help/%1/index.html").arg(lang));
    myHelpWin->show();
}

void MainWindow::closeEvent(QCloseEvent *evt) {
    settings.save(this);
}
