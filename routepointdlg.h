#ifndef ROUTEPOINTDLG_H
#define ROUTEPOINTDLG_H

#include <QDialog>
#include <QStringList>
#include "mapicon.h"
#include "route.h"

class QComboBox;
class QLabel;
class QLineEdit;

class RoutePointDlg : public QDialog
{
    Q_OBJECT
private:
    QComboBox *eSym;
    QLineEdit *eName;
    QLineEdit *eDesc;
    QLineEdit *eLink;
    QLabel *eErr;

    RoutePoint myPoint;
    QStringList *mySymbols;

    void createSymList(QComboBox *box, const QList<MapIcon>& icons);
public:
    explicit RoutePointDlg(const RoutePoint& point, const QList<MapIcon>& icons, QWidget *parent = 0);
    RoutePoint point() const;
signals:
    
public slots:
private slots:
    void check();
};

#endif // ROUTEPOINTDLG_H
