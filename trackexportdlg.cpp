#include <QtWidgets>
#include "trackexportdlg.h"

TrackExportDlg::TrackExportDlg(const QString &exportDir, const QString &trackFileName, QWidget *parent) :
    QDialog(parent), myExportDir(exportDir), myTrackFileName(trackFileName)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *ctrl = new QGridLayout();
    QLabel *lExportDir = new QLabel(tr("E&xport Directory:"));
    ctrl->addWidget(lExportDir, 0, 0);
    eExportDir = new QLabel(myExportDir);
    ctrl->addWidget(eExportDir, 0, 1);
    exportDirAction = new QAction(QIcon(":/icons/disk.png"), tr("Select Export Directory"), this);
    connect(exportDirAction, SIGNAL(triggered()), this, SLOT(selExportDir()));
    QToolButton *bExportDir = new QToolButton();
    bExportDir->setDefaultAction(exportDirAction);
    lExportDir->setBuddy(bExportDir);
    ctrl->addWidget(bExportDir, 0, 2);
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
    QFileInfo fi(myTrackFileName);
    return myExportDir+"/"+fi.fileName();
}

QString TrackExportDlg::osmFileName() const {
    QFileInfo fi(myTrackFileName);
    return myExportDir+"/"+fi.baseName()+"_osm.html";
}

void TrackExportDlg::selExportDir() {
    QString fileName = QFileDialog::getExistingDirectory(this,
                                                         tr("HTML export Directory"),
                                                         myExportDir);
    if (!fileName.isEmpty()) {
        //eFileName->setText(fileName);
        myExportDir = fileName;
        eExportDir->setText(myExportDir);
        eOsmFile->setText(osmFileName());
    }
    this->update();
}

