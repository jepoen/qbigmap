#include <QtGui>
#include "deltrkpartdlg.h"
#include "mapscene.h"
#include "model.h"
#include "track.h"

DelTrkPartDlg::DelTrkPartDlg(MapScene *scene, QWidget *parent) :
    QDialog(parent), myScene(scene)
{
    myModel = scene->model();
    myTrack = myModel->trackPtr();
    int pos = myTrack->pos();
    int max = myTrack->trackPoints().size()-1;
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lFrom = new QLabel(tr("&From position:"));
    eFrom = new QSpinBox();
    eFrom->setRange(0, max);
    eFrom->setValue(pos);
    lFrom->setBuddy(eFrom);
    control->addWidget(lFrom, 0, 0);
    control->addWidget(eFrom, 0, 1);
    eTo = new QSpinBox();
    eTo->setRange(0, max);
    eTo->setValue(pos);
    control->addWidget(eTo, 1, 1);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    QPointF p = myTrack->trackPoint(pos).coord();
    myItem = new QGraphicsLineItem();
    myItem->setLine(QLineF(myModel->lonLat2Scene(p), myModel->lonLat2Scene(p)));
    myItem->setPen(QPen(QBrush(Qt::magenta), 3));
    myScene->addItem(myItem);
    connect(eFrom, SIGNAL(valueChanged(int)), this, SLOT(setFrom(int)));
    connect(eTo, SIGNAL(valueChanged(int)), this, SLOT(setTo(int)));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
}

DelTrkPartDlg::~DelTrkPartDlg() {
    delete myItem;
}

void DelTrkPartDlg::setFrom(int pos) {
    int to = eTo->value();
    if (pos > to) {
        pos = to;
        eFrom->setValue(pos);
    }
    updateItem(pos, to);
}

void DelTrkPartDlg::setTo(int pos) {
    int from = eFrom->value();
    if (pos < from) {
        pos = from;
        eTo->setValue(pos);
    }
    updateItem(from, pos);
}

void DelTrkPartDlg::updateItem(int i0, int i1) {
    QPointF p0 = myTrack->trackPoint(i0).coord();
    QPointF p1 = myTrack->trackPoint(i1).coord();
    myItem->setLine(QLineF(myModel->lonLat2Scene(p0), myModel->lonLat2Scene(p1)));
}

int DelTrkPartDlg::pos0() const {
    return eFrom->value();
}

int DelTrkPartDlg::pos1() const {
    return eTo->value();
}
