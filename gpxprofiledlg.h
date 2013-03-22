#ifndef GPXPROFILEDLOG_H
#define GPXPROFILEDLOG_H

#include <QDialog>
#include <QSize>
#include "gpx.h"

class QSpinBox;
class QCheckBox;

class GpxProfileDlg : public QDialog
{
    Q_OBJECT
private:
    GpxPointList myGpxPoints;
    double myScaleX;
    double myScaleY;
    QSpinBox *eWidth;
    QSpinBox *eHeight;
    QCheckBox *eText;
    QSpinBox *eTextWidth;

    void computePreferredSize();
public:
    explicit GpxProfileDlg(const GpxPointList& gpxPoints, QWidget *parent = 0);
    QSize pixmapSize() const;
    int textWidth() const;
signals:
    
public slots:
    
};

#endif // GPXPROFILEDLOG_H
