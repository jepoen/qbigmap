#ifndef TRACKPOSDLG_H
#define TRACKPOSDLG_H

#include <QDialog>
#include "track.h"

class QDateTimeEdit;
class QDoubleSpinBox;
class QLineEdit;

class TrackPosDlg : public QDialog
{
    Q_OBJECT
private:
    ExtTrackPoint myP;
    QDoubleSpinBox *eLon;
    QDoubleSpinBox *eLat;
    QDoubleSpinBox *eElevation;
    QDateTimeEdit *eTimestamp;

public:
    explicit TrackPosDlg(const ExtTrackPoint& p, QWidget *parent = 0);
    const ExtTrackPoint& extTrackPoint() const;
signals:
    void posChanged(const QPointF& pos);

private slots:
    void changePos(double val);
    void accept();

};

#endif // TRACKPOSDLG_H
