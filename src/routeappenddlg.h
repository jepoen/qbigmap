#ifndef ROUTEAPPENDDLG_H
#define ROUTEAPPENDDLG_H

#include "gpx.h"

class QCheckBox;

class RouteAppendDlg : public QDialog {
private:
    double myDistDirect;
    double myDistReverse;
    QCheckBox *bReverse;
    const GpxPointList *myOldPoints;
    const GpxPointList *myNewPoints;
public:
    RouteAppendDlg(const GpxPointList* oldPoints, const GpxPointList *newPoints);
    bool isReversed() const;
    double gap() const;
signals:

public slots:
};

#endif // ROUTEAPPENDDLG_H
