#ifndef PRINTSELDIALOG_H
#define PRINTSELDIALOG_H

#include <QDialog>

class QCheckBox;
class QLineEdit;

class OutputSelDlg : public QDialog
{
    Q_OBJECT
private:
    QCheckBox *eOverlays;
    QCheckBox *eGrid;
    QCheckBox *eTrack;
    QCheckBox *eTrackSym;
    QCheckBox *eRoute;
    QCheckBox *eRouteSym;
    QCheckBox *eWptSym;
    QCheckBox *eRestrict;
    QLineEdit *eCopy;

public:
    explicit OutputSelDlg(QWidget *parent = 0);
    bool getOverlays() const;
    bool getGrid() const;
    void setGrid(bool bShowGrid);
    bool getTrack() const;
    void setTrack(bool bShowTrack);
    bool getTrackSym() const;
    void setTrackSym(bool bTrackSym);
    bool getRoute() const;
    void setRoute(bool bShowRoute);
    bool getRouteSym() const;
    void setRouteSym(bool bRouteSym);
    bool getWptSym() const;
    void setWptSym(bool bWptSym);
    bool getRestrict() const;
    void setRestrict(bool bRestrict);
    QString copy() const;
signals:

public slots:

};

#endif // PRINTSELDIALOG_H
