#ifndef ROUTEAPPENDDLG_H
#define ROUTEAPPENDDLG_H

#include "gpx.h"

class RouteAppendDlg : public QDialog {
private:
    const GpxPointList *myOldPoints;
    const GpxPointList *myNewPoints;
public:
    RouteAppendDlg(const GpxPointList* oldPoints, const GpxPointList *newPoints);

signals:

public slots:
};

#endif // ROUTEAPPENDDLG_H
