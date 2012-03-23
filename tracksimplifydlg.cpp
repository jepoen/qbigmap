#include <QtGui>
#include "tracksimplifydlg.h"
#include "mapscene.h"
#include "track.h"

TrackSimplifyDlg::TrackSimplifyDlg(MapScene *scene, QWidget *parent) :
        QDialog(parent),
        myScene(scene),
        mySimpleTrack(0), myTrackItem(0)
{
    Track *track = myScene->model()->track();
    QString fileName = getSimpleFileName(track->fileName());
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
    eTrackPoints = new QLabel(QString("%1").arg(track->extTrackPoints().size()));
    ctrl->addWidget(eTrackPoints, 1, 1);
    QLabel *lFileName = new QLabel(tr("&Simple Track file name:"));
    ctrl->addWidget(lFileName, 2, 0);
    eFileName = new QLabel(fileName);
    ctrl->addWidget(eFileName, 2, 1);
    QToolButton *bFileName = new QToolButton();
    bFileName->setDefaultAction(selFileAction);
    lFileName->setBuddy(bFileName);
    ctrl->addWidget(bFileName, 2, 2);
    QLabel *lExport = new QLabel(tr("Export"));
    ctrl->addWidget(lExport, 3, 0);
    QHBoxLayout *expLayout = new QHBoxLayout();
    eOsm = new QCheckBox(tr("&OpenStreetMap"));
    expLayout->addWidget(eOsm);
    eGoogle = new QCheckBox(tr("&Google Maps"));
    expLayout->addWidget(eGoogle);
    ctrl->addLayout(expLayout, 3, 1, 1, 2);
    mainLayout->addLayout(ctrl);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(selFileAction, SIGNAL(triggered()), this, SLOT(selFileName()));
    connect(eFailure, SIGNAL(valueChanged(int)), this, SLOT(simplify(int)));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    simplify(0);
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

bool TrackSimplifyDlg::isOsm() const {
    return eOsm->isChecked();
}

bool TrackSimplifyDlg::isGoogle() const {
    return eGoogle->isChecked();
}

bool TrackSimplifyDlg::writeXml() const {
    QFile file(eFileName->text());
    if (!file.open(QFile::WriteOnly|QFile::Text)) return false;
    mySimpleTrack->writeModifiedXml(&file);
    file.close();
    return true;
}

void TrackSimplifyDlg::selFileName() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Simple Track File"), eFileName->text(),
                                                    tr("GPX Files (*.gpx)"));
    if (!fileName.isEmpty()) {
        eFileName->setText(fileName);
    }
}

void TrackSimplifyDlg::simplify(int val) {
    qDebug()<<"simplify "<<val;
    mySimpleTrack.reset(myScene->model()->track()->simplify(val));
    eTrackPoints->setText(QString("%1").arg(mySimpleTrack->extTrackPoints().size()));
    redrawTrack();
}

void TrackSimplifyDlg::redrawTrack() {
    QPolygonF points;
    Model *myModel = myScene->model();
    foreach (ExtTrackPoint p, mySimpleTrack->extTrackPoints()) {
        QPointF pt = myModel->lonLat2Scene(p.lonLat());
        points.append(pt);
    }
    if (!myTrackItem.get()) {
        myTrackItem.reset(new TrackItem(points));
        myTrackItem->setColor(Qt::cyan);
        myScene->addItem(myTrackItem.get());
    }
    else {
        myTrackItem->setPoints(points);
    }
}
