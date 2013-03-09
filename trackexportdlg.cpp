#include <QtGui>
#include "trackexportdlg.h"

TrackExportDlg::TrackExportDlg(const QString &trackFileName, QWidget *parent) :
    QDialog(parent), myTrackFileName(QFileInfo(trackFileName).absoluteFilePath())
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *ctrl = new QGridLayout();
    QLabel *lTrkFile = new QLabel(tr("Track File:"));
    ctrl->addWidget(lTrkFile, 0, 0);
    eTrkFile = new QLabel(myTrackFileName);
    ctrl->addWidget(eTrkFile, 0, 1);
    eHasWpts = new QCheckBox("Add &waypoints");
    ctrl->addWidget(eHasWpts, 1, 0, 1, 2);
    eOsm = new QCheckBox(tr("&OpenStreetMap export"));
    eOsm->setChecked(true);
    ctrl->addWidget(eOsm, 2, 0, 1, 2);
    eSimple = new QCheckBox(tr("&Remove timestamp/elevation"));
    eSimple->setChecked(true);
    ctrl->addWidget(eSimple, 3, 0, 1, 2);
    QLabel *lTitle = new QLabel(tr("Map &title:"));
    ctrl->addWidget(lTitle, 4, 0);
    eTitle = new QLineEdit(tr("Track"));
    lTitle->setBuddy(eTitle);
    ctrl->addWidget(eTitle, 4, 1);
    QLabel *lOsmFile = new QLabel("OSM File:");
    ctrl->addWidget(lOsmFile, 5, 0);
    eOsmFile = new QLabel(osmFileName());
    ctrl->addWidget(eOsmFile, 5, 1);
    mainLayout->addLayout(ctrl);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

bool TrackExportDlg::isOsm() const {
    return eOsm->isChecked();
}

bool TrackExportDlg::isSimple() const {
    return eSimple->isChecked();
}

void TrackExportDlg::setWpts(bool val) {
    eHasWpts->setChecked(val);
}

bool TrackExportDlg::hasWpts() const {
    return eHasWpts->isChecked();
}

QString TrackExportDlg::title() const {
    return eTitle->text();
}

QString TrackExportDlg::fileName() const {
    return myTrackFileName;
}

QString TrackExportDlg::osmFileName() const {
    QFileInfo fi(myTrackFileName);
    return fi.absoluteDir().absoluteFilePath(QFileInfo(fi).baseName()+"_osm.html");
}

void TrackExportDlg::selFileName() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Simple Track File"), myTrackFileName,
                                                    tr("GPX Files (*.gpx)"));
    if (!fileName.isEmpty()) {
        //eFileName->setText(fileName);
        eOsmFile->setText(osmFileName());
    }
    this->update();
}

