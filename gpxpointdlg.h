#ifndef GPXPOINTDLG_H
#define GPXPOINTDLG_H

#include <QDialog>
#include <QStringList>
#include "mapicon.h"
#include "gpx.h"

class QComboBox;
class QLabel;
class QLineEdit;

class GpxPointDlg : public QDialog
{
    Q_OBJECT
private:
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

    void createSymList(QComboBox *box, const QList<MapIcon>& icons);
public:
    explicit GpxPointDlg(const GpxPoint& point, const QList<MapIcon>& icons, QWidget *parent = 0);
    void setDists(double dist0, double dist1);
    void setSrtmEle(int ele);
    GpxPoint point() const;
signals:
    
public slots:
private slots:
    void check();
};

#endif // GPXPOINTDLG_H
