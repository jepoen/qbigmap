#ifndef PRINTSELDIALOG_H
#define PRINTSELDIALOG_H

#include <QDialog>

class QCheckBox;

class OutputSelDlg : public QDialog
{
    Q_OBJECT
private:
    QCheckBox *eOverlays;
    QCheckBox *eGrid;
    QCheckBox *eTrack;
    QCheckBox *eRoute;
    QCheckBox *eRestrict;

public:
    explicit OutputSelDlg(QWidget *parent = 0);
    bool getOverlays() const;
    bool getGrid() const;
    void setGrid(bool bShowGrid);
    bool getTrack() const;
    void setTrack(bool bShowTrack);
    bool getRoute() const;
    void setRoute(bool bShowRoute);
    void setRestrict(bool bRestrict);
    bool getRestrict() const;

signals:

public slots:

};

#endif // PRINTSELDIALOG_H
