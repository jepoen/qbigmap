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
    GpxPoint myP;
    QDoubleSpinBox *eLon;
    QDoubleSpinBox *eLat;
    QDoubleSpinBox *eElevation;
    QDateTimeEdit *eTimestamp;

public:
    explicit TrackPosDlg(const GpxPoint& p, QWidget *parent = 0);
    const GpxPoint& trackPoint() const;
signals:
    void posChanged(const QPointF& pos);

private slots:
    void changePos(double val);
    void accept();

};

#endif // TRACKPOSDLG_H
