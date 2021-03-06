#include <QtWidgets>
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
    QLabel *lTo = new QLabel(tr("&To position:"));
    eTo = new QSpinBox();
    eTo->setRange(0, max);
    eTo->setValue(pos);
    lTo->setBuddy(eTo);
    control->addWidget(lTo, 1, 0);
    control->addWidget(eTo, 1, 1);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    myOldItem = new QGraphicsPathItem();
    myOldItem->setZValue(200);
    myOldItem->setPen(QPen(QBrush(Qt::red), 3));
    myNewItem = new QGraphicsLineItem();
    myNewItem->setZValue(21);
    myNewItem->setPen(QPen(QBrush(Qt::green), 3));
    myScene->addItem(myNewItem);
    updateItem(pos, pos);
    connect(eFrom, SIGNAL(valueChanged(int)), this, SLOT(setFrom(int)));
    connect(eTo, SIGNAL(valueChanged(int)), this, SLOT(setTo(int)));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
}

DelTrkPartDlg::~DelTrkPartDlg() {
    delete myOldItem;
    delete myNewItem;
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
    QPointF p0, p1;
    if (i0 > 0 && i1 < myTrack->trackPoints().size()-1) {
        p0 = myTrack->trackPoint(i0-1).coord();
        p1 = myTrack->trackPoint(i1+1).coord();
    } else {
        p0 = p1 = myTrack->trackPoint(0).coord();
    }
    myNewItem->setLine(QLineF(myModel->lonLat2Scene(p0), myModel->lonLat2Scene(p1)));
    QPainterPath path;
    bool start = true;
    for (int i = i0-1; i <= i1+1; i++) {
        if (i < 0 || i >= myTrack->trackPoints().size()) continue;
        const QPointF& p = myTrack->trackPoint(i).coord();
        if (start) {
            path.moveTo(myModel->lonLat2Scene(p));
            start = false;
        } else {
            path.lineTo(myModel->lonLat2Scene(p));
        }
    }
    myScene->removeItem(myOldItem);
    myOldItem->setPath(path);
    myScene->addItem(myOldItem);
}

int DelTrkPartDlg::pos0() const {
    return eFrom->value();
}

int DelTrkPartDlg::pos1() const {
    return eTo->value();
}
