#include <QtWidgets>
#include "routeappenddlg.h"
#include "model.h"

RouteAppendDlg::RouteAppendDlg(const GpxPointList *oldPoints, const GpxPointList *newPoints):
    myOldPoints(oldPoints), myNewPoints(newPoints)
{
    double distDirect = 0.0;
    double distReverse = 0.0;
    if (oldPoints->size() > 0) {
        distDirect = Model::geodist1(myOldPoints->last().coord(), myNewPoints->first().coord());
        distReverse = Model::geodist1(myOldPoints->last().coord(), myNewPoints->last().coord());
    }
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *lDists = new QLabel(tr("Distance to first point: %1 km to last point: %2 km").arg(distDirect).arg(distReverse));
    mainLayout->addWidget(lDists);
    setLayout(mainLayout);
}

