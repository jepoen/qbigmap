#include <QtWidgets>
#include "routeappenddlg.h"
#include "model.h"

RouteAppendDlg::RouteAppendDlg(const GpxPointList *oldPoints, const GpxPointList *newPoints):
    myOldPoints(oldPoints), myNewPoints(newPoints)
{
    myDistDirect = 0.0;
    myDistReverse = 0.0;
    if (oldPoints->size() > 0) {
        myDistDirect = Model::geodist1(myOldPoints->last().coord(), myNewPoints->first().coord());
        myDistReverse = Model::geodist1(myOldPoints->last().coord(), myNewPoints->last().coord());
    }
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *lDist1 = new QLabel(tr("Distance to first point: %1 km").arg(myDistDirect));
    mainLayout->addWidget(lDist1);
    QLabel *lDist2 = new QLabel(tr("Distance to last point: %1 km").arg(myDistReverse));
    mainLayout->addWidget(lDist2);
    bReverse = new QCheckBox(tr("Reverse new part"));
    mainLayout->addWidget(bReverse);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    if (myDistReverse < myDistDirect) {
        bReverse->setChecked(true);
    }
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

bool RouteAppendDlg::isReversed() const {
    return bReverse->isChecked();
}

double RouteAppendDlg::gap() const {
    if (bReverse->isChecked()) return myDistReverse;
    else return myDistDirect;
}

