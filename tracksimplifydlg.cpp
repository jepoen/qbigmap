#include <QtGui>
#include "geom.h"
#include "tracksimplifydlg.h"
#include "mapscene.h"
#include "track.h"

TrackSimplifyDlg::TrackSimplifyDlg(MapScene *scene, QWidget *parent) :
        QDialog(parent),
        myScene(scene),
        mySimpleTrack(0), myTrackItem(0)
{
    const Track& track = myScene->model()->track();
    QString fileName = getSimpleFileName(track.fileName());
    selFileAction = new QAction(QIcon(":/icons/disk.png"), tr("Select"), this);
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
    QLabel *lFileName = new QLabel(tr("&Simple Track file name:"));
    ctrl->addWidget(lFileName, 2, 0);
    eFileName = new QLabel(fileName);
    ctrl->addWidget(eFileName, 2, 1);
    QToolButton *bFileName = new QToolButton();
    bFileName->setDefaultAction(selFileAction);
    lFileName->setBuddy(bFileName);
    ctrl->addWidget(bFileName, 2, 2);
    eOsm = new QCheckBox(tr("&OpenStreetMap export"));
    eOsm->setChecked(true);
    ctrl->addWidget(eOsm, 3, 0);
    QLabel *lTitle = new QLabel(tr("Map &title:"));
    ctrl->addWidget(lTitle, 4, 0);
    eTitle = new QLineEdit(tr("Track"));
    lTitle->setBuddy(eTitle);
    ctrl->addWidget(eTitle, 4, 1);
    eOsmFile = new QLabel(osmFileName());
    ctrl->addWidget(eOsmFile, 5, 1);
    mainLayout->addLayout(ctrl);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(selFileAction, SIGNAL(triggered()), this, SLOT(selFileName()));
    connect(eFailure, SIGNAL(valueChanged(int)), this, SLOT(simplify(int)));
    connect(box, SIGNAL(accepted()), this, SLOT(finish()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    initLine();
    simplify(0);
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

QString TrackSimplifyDlg::getSimpleFileName(const QString &s) {
    int pos = s.lastIndexOf(".");
    if (pos >= 0) {
        return s.left(pos)+"_s.gpx";
    }
    else {
        return s+"_s.gpx";
    }
}

QString TrackSimplifyDlg::fileName() const {
    return eFileName->text();
}

QString TrackSimplifyDlg::osmFileName() const {
    QFileInfo fi(fileName());
    return fi.absoluteDir().absoluteFilePath(QFileInfo(fi).baseName()+"_osm.html");
}

QString TrackSimplifyDlg::title() const {
    return eTitle->text();
}

bool TrackSimplifyDlg::isOsm() const {
    return eOsm->isChecked();
}

void TrackSimplifyDlg::selFileName() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Simple Track File"), eFileName->text(),
                                                    tr("GPX Files (*.gpx)"));
    if (!fileName.isEmpty()) {
        eFileName->setText(fileName);
        eOsmFile->setText(osmFileName());
    }
    this->update();
}

void TrackSimplifyDlg::simplify(int val) {
    qDebug()<<"simplify "<<val<<" eps "<<val*myScene->model()->mercUnitsM(myCenter);
    const Track& track = myScene->model()->track();
    QList<int> res = simplifyLine(myLine, myStopNodes, val*myScene->model()->mercUnitsM(myCenter));
    QList<GpxPoint> points;
    foreach (int idx, res) {
        const GpxPoint& p = track.trackPoints()[idx];
        points.push_back(p);
    }
    mySimpleTrack.reset(new Track(points));
    eTrackPoints->setText(QString("%1").arg(mySimpleTrack->trackPoints().size()));
    redrawTrack();
}

void TrackSimplifyDlg::finish() {
    QFile file(eFileName->text());
    if (file.exists()) {
        if (QMessageBox::warning(this, tr("File exists"), tr("File %1 exists. Overwrite it?").arg(file.fileName()),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
    }
    emit accept();
}

void TrackSimplifyDlg::redrawTrack() {
    QPolygonF points;
    Model *myModel = myScene->model();
    foreach (const GpxPoint& p, mySimpleTrack->trackPoints()) {
        QPointF pt = myModel->lonLat2Scene(p.coord());
        points.append(pt);
    }
    if (!myTrackItem.get()) {
        myTrackItem.reset(new TrackItem(points));
        myTrackItem->setColor(Qt::magenta);
        myScene->addItem(myTrackItem.get());
    }
    else {
        myTrackItem->setPoints(points);
    }
}
