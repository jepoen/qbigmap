#include <QtGui>
#include <QtDebug>
#include "centerdialog.h"
#include "mainwindow.h"
#include "model.h"
#include "mapscene.h"
#include "mapview.h"
#include "profilescene.h"
#include "profileview.h"
#include "layersdialog.h"
#include "outputseldlg.h"
#include "photo.h"
#include "photodlg.h"
#include "saveroutedlg.h"
#include "settingsdialog.h"
#include "track.h"
#include "route.h"
#include "trackposdlg.h"
#include "trackseldialog.h"
#include "tracksimplifydlg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    myPhotoOffset(0)
{
    readSettings();
    scene = new MapScene(model);
    scene->setSceneRect(0, 0, 256*model->width(), 256*model->height());
    view = new MapView(scene);
    profileScene = new ProfileScene(model);
    profileView = new ProfileView(profileScene);
    profileView->setVisible(false);
    createActions();
    createMenuBar();
    createToolBar();
    createStatusBar();
    printer = new QPrinter();
    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal);
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical);
    horizontalSplitter->addWidget(verticalSplitter);
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
    verticalSplitter->addWidget(center);
    verticalSplitter->addWidget(profileView);
    createPhotoWidget();
    horizontalSplitter->addWidget(photoWidget);
    setCentralWidget(horizontalSplitter);
    createBaselayerActions();
    createOverlayActions();
    disableTrackActions();
    // should draw the scene
    changeOverlays();
    toggleGrid();
    connect(model, SIGNAL(mapChanged()), this, SLOT(updateModelStatus()));
    connect(model, SIGNAL(trackPosChanged(int)), this, SLOT(changeTrackPos(int)));
    connect(view, SIGNAL(mouseGeoPos(QPointF)), this, SLOT(showGeoPos(QPointF)));
    connect(photoList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showPhotoData(QListWidgetItem*)));
    connect(photoList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showPhoto(QListWidgetItem*)));
    updateModelStatus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::readSettings() {
    settings.load();
    model = new Model(settings.baseLayers()[0], settings.center());
    model->setZoom(settings.zoom());
}

void MainWindow::createPhotoWidget() {
    photoWidget = new QWidget();
    QGridLayout *photoLayout = new QGridLayout();
    photoList = new QListWidget();
    photoList->setViewMode(QListView::IconMode);
    photoList->setIconSize(QSize(200, 200));
    photoList->setMinimumWidth(210);
    photoLayout->addWidget(photoList);
    photoWidget->setLayout(photoLayout);
    photoWidget->setVisible(false);
}

void MainWindow::createActions() {
    printAction = new QAction(tr("&Print..."), this);
    printAction->setIcon(QIcon(":/icons/printer.png"));
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));
    savePixmapAction = new QAction(tr("Save pixmap..."), this);
    quitAction = new QAction(tr("Quit"), this);
    trackFromGpsAction = new QAction(tr("Read GPS device"), this);
    connect(trackFromGpsAction, SIGNAL(triggered()), this, SLOT(readTrackFromGps()));
    loadTrackAction = new QAction(tr("Load track..."), this);
    connect(loadTrackAction, SIGNAL(triggered()), this, SLOT(loadTrack()));
    saveTrackAction = new QAction(QIcon(":/icons/disk.png"), tr("Save..."), this);
    connect(saveTrackAction, SIGNAL(triggered()), this, SLOT(saveTrack()));
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
    editTrackPosAction = new QAction(tr("&Edit track position..."), this);
    connect(editTrackPosAction, SIGNAL(triggered()), this, SLOT(editTrackPos()));
    deleteTrackPosAction = new QAction(tr("&Delete track position"), this);
    connect(deleteTrackPosAction, SIGNAL(triggered()), this, SLOT(deleteTrackPos()));
    redrawAction = new QAction(tr("&Redraw"), this);
    redrawAction->setIcon(QIcon(":/icons/arrow_refresh.png"));
    QActionGroup *functionActionGroup = new QActionGroup(this);
    posAction = new QAction(QIcon(":/icons/information.png"), tr("Tile Position"), this);
    posAction->setCheckable(true);
    posAction->setChecked(true);
    functionActionGroup->addAction(posAction);
    zoomInAction = new QAction(QIcon(":/icons/zoom_in.png"), tr("Zoom in"), this);
    zoomInAction->setCheckable(true);
    functionActionGroup->addAction(zoomInAction);
    zoomOutAction = new QAction(QIcon(":/icons/zoom_out.png"), tr("Zoom out"), this);
    zoomOutAction->setCheckable(true);
    functionActionGroup->addAction(zoomOutAction);
    openRouteAction = new QAction(tr("Open route..."), this);
    connect(openRouteAction, SIGNAL(triggered()), this, SLOT(openRoute()));
    newRoutePointAction = new QAction(tr("New route point"), functionActionGroup);
    newRoutePointAction->setCheckable(true);
    connect(newRoutePointAction, SIGNAL(triggered()), view, SLOT(setNewRoutePointFunction()));
    delRoutePointAction = new QAction(tr("Delete route point"), this);
    delRoutePointAction->setCheckable(true);
    functionActionGroup->addAction(delRoutePointAction);
    connect(delRoutePointAction, SIGNAL(triggered()), view, SLOT(setDelRoutePointFunction()));
    moveRoutePointAction = new QAction(tr("Move route point"), this);
    moveRoutePointAction->setCheckable(true);
    functionActionGroup->addAction(moveRoutePointAction);
    connect(moveRoutePointAction, SIGNAL(triggered()), view, SLOT(setMoveRoutePointFunction()));
    saveRouteAction = new QAction(tr("Save as"), this);
    connect(saveRouteAction, SIGNAL(triggered()), this, SLOT(saveRoute()));
    delRouteAction = new QAction(tr("Delete complete route"), this);
    connect(delRouteAction, SIGNAL(triggered()), this, SLOT(delRoute()));
    showPhotoAction = new QAction(tr("Show photos..."), this);
    connect(showPhotoAction, SIGNAL(triggered()), this, SLOT(showPhotos()));
    hidePhotoAction = new QAction(tr("Hide photos"), this);
    connect(hidePhotoAction, SIGNAL(triggered()), this, SLOT(hidePhotos()));
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
    editLayersAction = new QAction(tr("Edit base layers..."), this);
    editOverlaysAction = new QAction(tr("Edit overlays..."), this);
    editSettingsAction = new QAction(tr("Edit settings..."), this);
    connect(savePixmapAction, SIGNAL(triggered()), this, SLOT(savePixmap()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(deleteTrackAction, SIGNAL(triggered()), this, SLOT(deleteTrack()));
    connect(lastTrackPosAction, SIGNAL(triggered()), this, SLOT(lastTrackPos()));
    connect(redrawAction, SIGNAL(triggered()), scene, SLOT(redraw()));
    connect(posAction, SIGNAL(triggered()), view, SLOT(setShowFunction()));
    connect(zoomInAction, SIGNAL(triggered()), view, SLOT(setZoomInFunction()));
    connect(zoomOutAction, SIGNAL(triggered()), view, SLOT(setZoomOutFunction()));
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

void MainWindow::disableTrackActions() {
    saveTrackAction->setEnabled(false);
    firstTrackPosAction->setEnabled(false);
    decTrackPosAction->setEnabled(false);
    dDecTrackPosAction->setEnabled(false);
    incTrackPosAction->setEnabled(false);
    dIncTrackPosAction->setEnabled(false);
    lastTrackPosAction->setEnabled(false);
    deleteTrackAction->setEnabled(false);
    trackBoundingBoxAction->setEnabled(false);
    trackSimplifyAction->setEnabled(false);
    editTrackPosAction->setEnabled(false);
    deleteTrackPosAction->setEnabled(false);
    lTrackPos->setVisible(false);
}

void MainWindow::enableTrackActions() {
    saveTrackAction->setEnabled(true);
    firstTrackPosAction->setEnabled(true);
    decTrackPosAction->setEnabled(true);
    dDecTrackPosAction->setEnabled(true);
    incTrackPosAction->setEnabled(true);
    dIncTrackPosAction->setEnabled(true);
    lastTrackPosAction->setEnabled(true);
    deleteTrackAction->setEnabled(true);
    trackBoundingBoxAction->setEnabled(true);
    trackSimplifyAction->setEnabled(true);
    editTrackPosAction->setEnabled(true);
    deleteTrackPosAction->setEnabled(true);
    lTrackPos->setVisible(true);
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
    mFile->addAction(printAction);
    mFile->addAction(savePixmapAction);
    mFile->addAction(setCenterAction);
    mFile->addAction((quitAction));
    QMenu *mTrack = menuBar()->addMenu(tr("&Track"));
    mTrack->addAction(trackFromGpsAction);
    mTrack->addAction(loadTrackAction);
    mTrack->addAction(saveTrackAction);
    mTrack->addAction(deleteTrackAction);
    mTrack->addAction(firstTrackPosAction);
    mTrack->addAction(decTrackPosAction);
    mTrack->addAction(dDecTrackPosAction);
    mTrack->addAction(incTrackPosAction);
    mTrack->addAction(dIncTrackPosAction);
    mTrack->addAction(lastTrackPosAction);
    mTrack->addAction(trackBoundingBoxAction);
    mTrack->addAction(trackSimplifyAction);
    mTrack->addSeparator();
    mTrack->addAction(editTrackPosAction);
    mTrack->addAction(deleteTrackPosAction);
    QMenu *mRoute = menuBar()->addMenu(tr("&Route"));
    mRoute->addAction(openRouteAction);
    mRoute->addAction(newRoutePointAction);
    mRoute->addAction(delRoutePointAction);
    mRoute->addAction(moveRoutePointAction);
    mRoute->addAction(saveRouteAction);
    mRoute->addAction(delRouteAction);
    QMenu *mView = menuBar()->addMenu(tr("&View"));
    mView->addAction(redrawAction);
    mView->addAction(showGridAction);
    mView->addAction(showTileBoundAction);
    mType = mView->addMenu(tr("&Type"));
    foreach (QAction *a, typeActionGroup->actions()) {
        mType->addAction(a);
    }

    mOverlays = mView->addMenu(tr("&Overlays"));
    foreach (QAction *a, ovlActionGroup->actions()) {
        mOverlays->addAction(a);
    }
    QMenu *mPhotos = menuBar()->addMenu(tr("&Photos"));
    mPhotos->addAction(showPhotoAction);
    mPhotos->addAction(hidePhotoAction);

    QMenu *mSettings = menuBar()->addMenu(tr("&Settings"));
    mSettings->addAction(editLayersAction);
    mSettings->addAction(editOverlaysAction);
    mSettings->addAction(editSettingsAction);
}

void MainWindow::createToolBar() {
    toolBar = addToolBar(tr("Tools"));
    toolBar->addAction(printAction);
    toolBar->addSeparator();
    toolBar->addAction(redrawAction);
    toolBar->addAction(posAction);
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
    toolBar->addAction(showGridAction);
    toolBar->addSeparator();
    trackToolBar = addToolBar(tr("Track"));
    trackToolBar->addAction(firstTrackPosAction);
    trackToolBar->addAction(dDecTrackPosAction);
    trackToolBar->addAction(decTrackPosAction);
    trackToolBar->addAction(editTrackPosAction);
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

void MainWindow::paintTrack(QPainter *painter) {
    QPen pen(settings.outTrackColor());
    pen.setWidth(settings.outTrackWidth());
    painter->setPen(pen);
    QPainterPath path;
    bool start = true;
    foreach(const TrackPoint& point, model->track()->trackPoints()) {
        QPoint p = model->lonLat2Scene(point.lonLat());
        if (start) {
            path.moveTo(p);
            start = false;
        } else {
            path.lineTo(p);
        }
    }
    painter->drawPath(path);
}

void MainWindow::paintRoute(QPainter *painter) {
    QPen pen(settings.outRouteColor());
    pen.setWidth(settings.outRouteWidth());
    painter->setPen(pen);
    QPainterPath path;
    bool start = true;
    foreach(const RoutePoint& point, *model->route()->points()) {
        QPoint p = model->lonLat2Scene(point.coord());
        if (start) {
            path.moveTo(p);
            start = false;
        } else {
            path.lineTo(p);
        }
    }
    painter->drawPath(path);
}

void MainWindow::paintGrid(QPainter *painter) {
    QPointF lonLat0 = model->lonLat(QPointF(0, 0));
    QPointF lonLat1 = model->lonLat(QPointF(0, model->height()*256));
    double distKm = Model::geodist0(lonLat0, lonLat1);
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

QPixmap* MainWindow::createPixmap() {
    OutputSelDlg dlg;
    dlg.setGrid(scene->isShowGrid());
    dlg.setTrack(scene->isShowTrack());
    dlg.setRoute(model->route()->points()->size() > 0);
    if (dlg.exec() != QDialog::Accepted) {
        return 0;
    }
    bool showOverlays = dlg.getOverlays();
    bool showGrid = dlg.getGrid();
    bool showTrack = dlg.getTrack();
    bool restrict = dlg.getRestrict();
    bool showRoute = dlg.getRoute();
    int w = model->width()*256;
    int h = model->height()*256;
    int x0 = 0;
    int y0 = 0;
    if (model->track() != 0 && restrict) {
        BoundingBox bb = model->track()->boundingBox();
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
    if (showTrack && model->track() != 0) {
        paintTrack(&painter);
    }
    if (showRoute && model->route() != 0) {
        paintRoute(&painter);
    }
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
        settings.setCenter(dlg.lonLat());
        settings.setZoom(model->zoom());
        settings.save();
        QMessageBox::information(this, tr("Save settings"),
                                 tr("Saved new center and zoom"),
                                 QMessageBox::Ok);
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
        settings.save();
        createBaselayerActions();
        model->setLayer(settings.baseLayers()[0]);
    }
}

void MainWindow::editOverlays() {
    LayersDialog dlg(settings.overlays());
    if (dlg.exec() == QDialog::Accepted) {
        qDebug()<<dlg.layers().size();
        settings.setOverlays(dlg.layers());
        settings.save();
        createOverlayActions();
        //model->setLayer(&baseLayers[0]);
    }

}

void MainWindow::editSettings() {
    SettingsDialog dlg(settings, this);
    if (dlg.exec() == QDialog::Accepted) {
        settings = dlg.settings();
        settings.save();
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
    Track *track = model->track();
    track = new Track(rawFileName);
    track->setFileName(filename);
    selectTrackSegments(track);
}

void MainWindow::loadTrack() {
    QString dir = settings.trackDir();
    Track *track = model->track();
    if (track != NULL) {
        QFileInfo fi(track->fileName());
        qDebug()<<"Dir: "<<fi.absoluteDir();
        dir = fi.absoluteDir().absolutePath();
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load track"), dir,
                                               tr("GPX track (*.gpx)"));
    if (fileName.isEmpty())
        return;
    track = new Track(fileName);
    selectTrackSegments(track);
}

void MainWindow::saveTrack() {
    QString fileName = model->track()->fileName();
    fileName = QFileDialog::getSaveFileName(this, tr("Save track"), fileName,
                                            tr("GPX track (*.gpx)"));
    if (fileName.isEmpty()) return;
    int idx = fileName.lastIndexOf(".");
    if (idx < 0) fileName +".gpx";
    QFile file(fileName);
    file.open(QFile::WriteOnly|QFile::Text);
    model->track()->writeModifiedXml(&file, false);
    file.close();
    model->track()->setFileName(fileName);
}

void MainWindow::selectTrackSegments(Track *track) {
    QList<SegInfo> segInfo = track->segmentInfo();
    TrackSelDialog dlg(segInfo);
    if (dlg.exec()) {
        QList<int> selSegs = dlg.selectedRows();
        if (selSegs.size() == 0) return;
        qDebug()<<"selected: "<<selSegs;
        track->selectSegments(selSegs);
        BoundingBox bb = track->boundingBox();
        QPointF center(0.5*(bb.p0().x()+bb.p1().x()), 0.5*(bb.p0().y()+bb.p1().y()));
        qDebug()<<"new center "<<center;
        model->setCenter(center);
        model->setTrack(track);
        model->setTrackPos(0);
        int pos = track->fileName().lastIndexOf("/");
        profileView->setVisible(true);
        trackToolBar->setVisible(true);
        enableTrackActions();
        setWindowTitle(tr("qbigmap - %1").arg(track->fileName().mid(pos+1)));
    }
}

void MainWindow::deleteTrack() {
    if (model->track() == NULL)
        return;
    if (QMessageBox::question(this, tr("Delete track"), tr("Delete current track?"),
                             QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::Yes) {
        model->setTrack(NULL);
        trackToolBar->setVisible(false);
        profileView->setVisible(false);
        connectPhotos();
        disableTrackActions();
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
    model->setTrackPos(model->track()->trackPoints().size()-1);
}

void MainWindow::changeTrackPos(int pos) {
    if (model->track() == NULL)
        return;
    ExtTrackPoint p = model->track()->extTrackPoint(pos);
    qDebug()<<"changed pos "<<p.lonLat();
    lTrackPos->setText(QString("%1 %2 (%3, %4) %5 mNN, %6 km")
                       .arg(pos)
                       .arg(p.timestamp().toLocalTime().toString("hh:mm"))
                       .arg(p.lonLat().x(), 0, 'f', 5)
                       .arg(p.lonLat().y(), 0, 'f', 5)
                       .arg(p.ele(), 0, 'f', 0)
                       .arg(p.sumDist(), 0, 'f', 1));
}

void MainWindow::toggleTrackBoundingBox() {
    scene->setShowTrackBb(trackBoundingBoxAction->isChecked());
}

void MainWindow::simplifyTrack() {
    TrackSimplifyDlg dlg(scene);
    if (dlg.exec() == QDialog::Accepted) {
        dlg.writeXml();
    }
}

void MainWindow::editTrackPos() {
    if (model->track() == NULL)
        return;
    int pos = model->track()->pos();
    ExtTrackPoint p = model->track()->extTrackPoint(pos);
    TrackPosDlg dlg(p);
    view->createTempPoint(p.lonLat());
    connect(&dlg, SIGNAL(posChanged(const QPointF&)), view, SLOT(moveTempPoint(QPointF)));
    if (dlg.exec() == QDialog::Accepted) {
        qDebug()<<"accepted";
        ExtTrackPoint point(dlg.extTrackPoint());
        model->setTrackPoint(pos, point);
    }
    view->deleteTempPoint();
}

void MainWindow::deleteTrackPos() {
    int pos = model->track()->pos();
    ExtTrackPoint p = model->track()->extTrackPoint(pos);
    if (QMessageBox::question(this, tr("Delete track point"),
                              tr("Delete the track point (%1,%2) %3")
                                .arg(p.lonLat().x()).arg(p.lonLat().y()).arg(p.timestamp().toString()),
                                QMessageBox::Ok|QMessageBox::Cancel)
        == QMessageBox::Ok) {
        model->delTrackPoint(pos);
    }
}

void MainWindow::openRoute() {
    if (model->route()->isDirty()) {
        // TODO: save old route
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Route"), QString(),
                                                    tr("GPX Route Files (*.gpx)"));
    if (!fileName.isEmpty()) {
        model->route()->readXml(fileName);
    }
}

void MainWindow::delRoute() {
    model->route()->delRoute();
}

void MainWindow::saveRoute() {
    SaveRouteDlg dlg(model->route()->name());
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
    model->route()->setName(name);
    model->route()->writeXml(&file);
    file.close();
    if (isUpload) {
        QStringList params;
        params<<"-r"
              <<"-i"<<"gpx"<<"-f"<<filename
              <<"-o"<<settings.gpsDevice()
              <<"-F"<<settings.gpsInterface();
        qDebug()<<settings.gpsbabel()<<params;
        QProcess gpsbabel;
        gpsbabel.start(settings.gpsbabel(), params);
        if (!gpsbabel.waitForStarted()) {
            QMessageBox::warning(this, tr("Cannot upload route"), tr("Cannot start gpsbabel"));
            return;
        }
        if (!gpsbabel.waitForFinished()) {
            QMessageBox::warning(this, tr("Cannot upload route"), tr("Cannot upload route %1: %2").arg(name)
                                 .arg(gpsbabel.errorString()));
            return;
        }
    }
}

void MainWindow::showPhotos() {
    QString sdir = QFileDialog::getExistingDirectory(this, tr("Select photo directory"));
    if (sdir.isEmpty()) return;
    photoList->clear();
    QDir dir(sdir);
    QStringList filter;
    filter<<"*.jpg";
    QStringList files = dir.entryList(filter, QDir::Files, QDir::Time);
    QList<Photo> photos;
    QProgressDialog progressDlg(tr("Download tiles"), tr("&Cancel"), 0, files.size(), this);
    //connect(&progressDlg, SIGNAL(canceled()), this, SLOT(cancelRequests()));
    //connect(&progressDlg, SIGNAL(destroyed()), this, SLOT(cancelRequests()));
    progressDlg.setWindowModality(Qt::WindowModal);
    progressDlg.show();
    progressDlg.setValue(0);
    qApp->processEvents();
    foreach (QString filename, files) {
        qApp->processEvents();
        if (progressDlg.isHidden()) break;
        filename = dir.absoluteFilePath(filename);
        Photo photo(filename);
        photos.push_back(photo);
        progressDlg.setValue(progressDlg.value()+1);
    }
    progressDlg.hide();
    qSort(photos);
    photoWidget->setVisible(true);
    foreach(Photo photo, photos) {
        QListWidgetItem *it = new QListWidgetItem();
        it->setIcon(photo.icon());
        it->setText(photo.timestamp().toString("yyyy-MM-dd HH:mm"));
        it->setToolTip(photo.filename());
        it->setData(PHOTO_FILENAME, photo.filename());
        it->setData(PHOTO_ORIGTIME, photo.timestamp());
        it->setData(PHOTO_TIMESTAMP, photo.timestamp().addSecs(myPhotoOffset));
        it->setData(PHOTO_TRACKIDX, -1);
        it->setBackgroundColor(Qt::yellow);
        photoList->addItem(it);
    }
    if (model->track() != 0) connectPhotos();
}

void MainWindow::connectPhotos() {
    Track *track = model->track();
    int cnt = photoList->count();
    for (int i = 0; i < cnt; i++) {
        QListWidgetItem *it = photoList->item(i);
        QDateTime timestamp = it->data(PHOTO_TIMESTAMP).toDateTime();
        it->setData(PHOTO_TRACKIDX, -1);
        it->setBackgroundColor(Qt::yellow);
        if (track != 0) {
            int pos = track->nearest(timestamp);
            if (abs(track->extTrackPoint(pos).timestamp().secsTo(timestamp)) < 300) {
                it->setData(Qt::UserRole+2, pos);
                it->setBackgroundColor(Qt::green);
            }
        }
    }
}

void MainWindow::hidePhotos() {
    photoWidget->setVisible(false);
    photoList->clear();
}

void MainWindow::showPhoto(QListWidgetItem *item) {
    QString filename = item->data(Qt::UserRole).toString();
    qDebug()<<filename;
    PhotoDlg dlg(filename);
    dlg.exec();
}

void MainWindow::showPhotoData(QListWidgetItem *item) {
    QString filename = item->data(PHOTO_FILENAME).toString();
    QDateTime timestamp = item->data(PHOTO_TIMESTAMP).toDateTime();
    qDebug()<<filename<<" "<<timestamp;
    int idx = item->data(PHOTO_TRACKIDX).toInt();
    if (idx >= 0) model->setTrackPos(idx);
}

void MainWindow::setPhotoOffset() {
    int cnt = photoList->count();
    for (int i = 0; i < cnt; i++) {
        QListWidgetItem *it = photoList->item(i);
        QDateTime timestamp = it->data(PHOTO_ORIGTIME).toDateTime();
        it->setData(PHOTO_TIMESTAMP, timestamp.addSecs(myPhotoOffset));
    }
    connectPhotos();
}
