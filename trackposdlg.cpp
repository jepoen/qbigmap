#include <QtWidgets>
#include <QtDebug>
#include "trackposdlg.h"

TrackPosDlg::TrackPosDlg(const GpxPoint& p, QWidget *parent) :
    QDialog(parent), myP(p)
{
    qDebug()<<"ele "<<myP.ele();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lLon = new QLabel(tr("L&ongitude:"));
    control->addWidget(lLon, 0, 0);
    eLon = new QDoubleSpinBox();
    eLon->setRange(-180, 180);
    eLon->setDecimals(6);
    eLon->setSingleStep(0.0001);
    eLon->setValue(myP.coord().x());
    lLon->setBuddy(eLon);
    control->addWidget(eLon, 0, 1);
    QLabel *lLat = new QLabel(tr("L&atitude:"));
    control->addWidget(lLat, 1, 0);
    eLat = new QDoubleSpinBox();
    eLat->setRange(-90, 90);
    eLat->setDecimals(6);
    eLat->setSingleStep(0.0001);
    eLat->setValue(myP.coord().y());
    lLat->setBuddy(eLat);
    control->addWidget(eLat, 1, 1);
    QLabel *lEle = new QLabel(tr("&Elevation:"));
    control->addWidget(lEle, 2, 0);
    eElevation = new QDoubleSpinBox();
    eElevation->setRange(-500, 9000);
    eElevation->setDecimals(1);
    eElevation->setValue(myP.ele());
    lEle->setBuddy(eElevation);
    control->addWidget(eElevation, 2, 1);
    QLabel *lTimestamp = new QLabel(tr("&Timestamp:"));
    control->addWidget(lTimestamp, 4, 0);
    eTimestamp = new QDateTimeEdit();
    eTimestamp->setDateTime(myP.timestamp());
    eTimestamp->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    //eTimestamp->setCalendarPopup(true);
    lTimestamp->setBuddy(eTimestamp);
    control->addWidget(eTimestamp, 4, 1);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(eLon, SIGNAL(valueChanged(double)), this, SLOT(changePos(double)));
    connect(eLat, SIGNAL(valueChanged(double)), this, SLOT(changePos(double)));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

void TrackPosDlg::changePos(double val) {
    Q_UNUSED(val);
    QPointF pos(eLon->value(), eLat->value());
    emit posChanged(pos);
}

const GpxPoint &TrackPosDlg::trackPoint() const {
    return myP;
}

void TrackPosDlg::accept() {
    myP.setCoord(QPointF(eLon->value(), eLat->value()));
    QDialog::accept();
}
