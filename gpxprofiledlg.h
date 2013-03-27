#ifndef GPXPROFILEDLOG_H
#define GPXPROFILEDLOG_H

#include <QDialog>
#include <QSize>
#include "gpx.h"
#include "gpxprofile.h"

class QLabel;
class QPixmap;
class QSpinBox;
class QCheckBox;
class Settings;

class GpxProfileDlg : public QDialog
{
    Q_OBJECT
private:
    GpxPointList myGpxPoints;
    GpxProfile myProfile;
    int myBorder;
    double myScaleX;
    double myScaleY;
    QLabel *eProfile;
    QSpinBox *eWidth;
    QSpinBox *eHeight;
    QCheckBox *eText;
    QSpinBox *eTop;
    QSpinBox *eTextWidth;

    void computePreferredSize();
public:
    explicit GpxProfileDlg(const GpxPointList& gpxPoints, Settings *settings, QWidget *parent = 0);
    const QPixmap *pixmap() const;
    QSize pixmapSize() const;
    int textWidth() const;
    int topOffset() const;
private slots:
    void fixWidth(bool val);
    void computePixmap();
};

#endif // GPXPROFILEDLOG_H
