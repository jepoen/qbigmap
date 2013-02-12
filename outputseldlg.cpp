#include <QtGui>
#include "outputseldlg.h"

OutputSelDlg::OutputSelDlg(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Output Map settings"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    eOverlays = new QCheckBox(tr("Include Overlays"));
    eOverlays->setChecked(true);
    control->addWidget(eOverlays, 0, 0);
    eGrid = new QCheckBox(tr("Include Grid"));
    control->addWidget(eGrid, 1, 0);
    eTrack = new QCheckBox(tr("Include Track"));
    control->addWidget(eTrack, 2, 0);
    eTrackSym = new QCheckBox(tr("Include Track Symbols"));
    control->addWidget(eTrackSym, 3, 0);
    eRoute = new QCheckBox(tr("Include Route"));
    control->addWidget(eRoute, 4, 0);
    eRouteSym = new QCheckBox(tr("Include Route Symbols"));
    control->addWidget(eRouteSym, 5, 0);
    eWptSym = new QCheckBox(tr("Include Waypoint Symbols"));
    control->addWidget(eWptSym, 6, 0);
    eRestrict = new QCheckBox(tr("Restrict Size to track"));
    control->addWidget(eRestrict, 7, 0);

    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

bool OutputSelDlg::getOverlays() const {
    return eOverlays->isChecked();
}

bool OutputSelDlg::getGrid() const {
    return eGrid->isChecked();
}

void OutputSelDlg::setGrid(bool bShowGrid) {
    eGrid->setChecked(bShowGrid);
}

void OutputSelDlg::setTrack(bool bShowTrack) {
    eTrack->setChecked(bShowTrack);
}

void OutputSelDlg::setTrackSym(bool bTrackSym) {
    eTrackSym->setChecked(bTrackSym);
}

void OutputSelDlg::setRoute(bool bShowRoute) {
    eRoute->setChecked(bShowRoute);
}

void OutputSelDlg::setRouteSym(bool bRouteSym) {
    eRouteSym->setChecked(bRouteSym);
}

void OutputSelDlg::setWptSym(bool bWptSym) {
    eWptSym->setChecked(bWptSym);
}

bool OutputSelDlg::getTrack() const {
    return eTrack->isChecked();
}

bool OutputSelDlg::getTrackSym() const {
    return eTrackSym->isChecked();
}

bool OutputSelDlg::getRoute() const {
    return eRoute->isChecked();
}

bool OutputSelDlg::getRouteSym() const {
    return eRouteSym->isChecked();
}

bool OutputSelDlg::getWptSym() const {
    return eWptSym->isChecked();
}

void OutputSelDlg::setRestrict(bool bRestrict) {
    eRestrict->setChecked(bRestrict);
}

bool OutputSelDlg::getRestrict() const {
    return eRestrict->isChecked();
}
