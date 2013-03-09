#include <QtGui>
#include <QtDebug>
#include "model.h"
#include "photooffsetdlg.h"

PhotoOffsetDlg::PhotoOffsetDlg(Model *model, const QDateTime &timestamp, int offset, QWidget *parent) :
    QDialog(parent),
    myModel(model), myTimestamp(timestamp), myOffset(offset)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lOffset = new QLabel(tr("&Offset (s):"));
    eOffset = new QSpinBox();
    eOffset->setRange(-MAXOFFSET, MAXOFFSET);
    eOffset->setValue(myOffset);
    eOffset->setSingleStep(10);
    lOffset->setBuddy(eOffset);
    control->addWidget(lOffset, 0, 0);
    control->addWidget(eOffset, 0, 1);
    eTimeDiff = new QLabel();
    control->addWidget(eTimeDiff, 2, 1);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(eOffset, SIGNAL(valueChanged(int)), this, SLOT(setOffset(int)));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    eOffset->setFocus();
    showTrackPos();
}


void PhotoOffsetDlg::showTrackPos() {
    if (myModel->isEmptyTrack()) return;
    qDebug()<<"timestamp "<<myTimestamp<<" offset "<<myOffset;
    const Track& track = myModel->track();
    if (track.trackPoints().size() == 0) return;
    int idx = track.nearest(myTimestamp.addSecs(myOffset));
    qDebug()<<"track idx "<<idx;
    myModel->setTrackPos(idx);
    qDebug()<<"track pos "<<idx<<track.trackPoint(idx).timestamp();
    QDateTime trackTime = track.trackPoint(idx).timestamp();
    myTimeDiff = trackTime.secsTo(myTimestamp.addSecs(myOffset));
    eTimeDiff->setText(QString("Trackpoint %1 Timestamp %2 diff %3s").arg(idx)
                       .arg(track.trackPoint(idx).timestamp().toLocalTime().toString("HH:mm:ss"))
                       .arg(myTimeDiff));
}

void PhotoOffsetDlg::setOffset(int offset) {
    myOffset = offset;
    showTrackPos();
}
