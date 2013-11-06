#include <QtGui>
#include "geom.h"
#include "tracksimplifydlg.h"
#include "mapscene.h"
#include "track.h"

TrackSimplifyDlg::TrackSimplifyDlg(MapScene *scene, QWidget *parent) :
        QDialog(parent),
        myScene(scene),
        myTrackItem(0),
        myAction(CANCEL)
{
    const Track& track = myScene->model()->track();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *ctrl = new QGridLayout();
    QLabel *lFailure = new QLabel(tr("&Failure (m):"));
    ctrl->addWidget(lFailure, 0, 0);
    eFailure = new QSpinBox();
    eFailure->setRange(0, 200);
    eFailure->setSingleStep(10);
    eFailure->setValue(0);
    lFailure->setBuddy(eFailure);
    ctrl->addWidget(eFailure, 0, 1);
    QLabel *lTrackPoints = new QLabel(tr("Track points"));
    ctrl->addWidget(lTrackPoints, 1, 0);
    eTrackPoints = new QLabel(QString("%1").arg(track.trackPoints().size()));
    ctrl->addWidget(eTrackPoints, 1, 1);
    mainLayout->addLayout(ctrl);
    bExport = new QPushButton(tr("E&xport"));
    connect(bExport, SIGNAL(clicked()), this, SLOT(exportTrk()));
    bReplace = new QPushButton(tr("&Replace"));
    connect(bReplace, SIGNAL(clicked()), this, SLOT(replaceTrk()));
    QDialogButtonBox *box = new QDialogButtonBox();
    box->addButton(bExport, QDialogButtonBox::ActionRole);
    box->addButton(bReplace, QDialogButtonBox::ActionRole);
    box->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(eFailure, SIGNAL(valueChanged(int)), this, SLOT(simplify(int)));
    connect(box, SIGNAL(rejected()), this, SLOT(cancel()));
    initLine();
    simplify(0);
}

TrackSimplifyDlg::~TrackSimplifyDlg() {
    //qDebug()<<"TrackSimplifyDlg destroyed";
    delete myTrackItem;
}

void TrackSimplifyDlg::initLine() {
    const Model* model = myScene->model();
    const Track& track = myScene->model()->track();
    foreach (const GpxPoint& p, track.trackPoints()) {
        myLine.push_back(model->lonLat2SpherMerc(p.coord()));
        bool stop = false;
        if (!p.sym().isNull() && !p.sym().isEmpty()) stop = true;
        myStopNodes.push_back(stop);
    }
    BoundingBox bb = track.boundingBox();
    double xm = bb.p0().x()+bb.p1().x();
    double ym = bb.p0().y()+bb.p1().y();
    myCenter = QPointF(0.5*xm, 0.5*ym);
    qDebug()<<"track center "<<myCenter;
}

void TrackSimplifyDlg::simplify(int val) {
    qDebug()<<"simplify "<<val<<" eps "<<val*myScene->model()->mercUnitsM(myCenter);
    const Track& track = myScene->model()->track();
    QList<int> res = simplifyLine(myLine, myStopNodes, val*myScene->model()->mercUnitsM(myCenter));
    mySimpleTrackPoints.clear();
    foreach (int idx, res) {
        const GpxPoint& p = track.trackPoints()[idx];
        mySimpleTrackPoints.push_back(p);
    }
    eTrackPoints->setText(QString("%1").arg(mySimpleTrackPoints.size()));
    redrawTrack();
}

void TrackSimplifyDlg::finish() {
    delete myTrackItem;
    myTrackItem = 0;
    emit accept();
}

void TrackSimplifyDlg::exportTrk() {
    myAction = EXPORT;
    finish();
}

void TrackSimplifyDlg::replaceTrk() {
    myAction = REPLACE;
    finish();
}

void TrackSimplifyDlg::redrawTrack() {
    QPolygonF points;
    Model *myModel = myScene->model();
    foreach (const GpxPoint& p, mySimpleTrackPoints) {
        QPointF pt = myModel->lonLat2Scene(p.coord());
        points.push_back(pt);
    }

    if (myTrackItem == 0) {
        myTrackItem = new TrackItem(points);
        myTrackItem->setColor(Qt::magenta);
        myScene->addItem(myTrackItem);
    }
    else {
        myTrackItem->setPoints(points);
    }
}

void TrackSimplifyDlg::cancel() {
    delete myTrackItem;
    myTrackItem = 0;
    reject();
}
