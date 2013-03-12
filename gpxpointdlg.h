#ifndef GPXPOINTDLG_H
#define GPXPOINTDLG_H

#include <QDialog>
#include <QStringList>
#include "mapicon.h"
#include "gpx.h"

class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;

class GpxPointDlg : public QDialog
{
    Q_OBJECT
private:
    QDoubleSpinBox *eLon;
    QDoubleSpinBox *eLat;
    QDateTimeEdit *eTimestamp;
    QLineEdit *eEle;
    QLabel *eSrtmEle;
    QComboBox *eSym;
    QLineEdit *eName;
    QLineEdit *eDesc;
    QLineEdit *eLink;
    QLabel *eDist0;
    QLabel *eDist1;
    QLabel *eErr;

    GpxPoint myPoint;
    QStringList *mySymbols;

    void createSymList(QComboBox *box, const MapIconList& iconList);
public:
    explicit GpxPointDlg(const GpxPoint& point, const MapIconList& icons, QWidget *parent = 0);
    void setDists(double dist0, double dist1);
    void setSrtmEle(int ele);
    GpxPoint point() const;
signals:
    void posChanged(const QPointF& pos);

public slots:
private slots:
    void changePos(double val);
    void check();
};

#endif // GPXPOINTDLG_H
