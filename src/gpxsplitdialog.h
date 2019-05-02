#ifndef GPXSPLITDIALOG_H
#define GPXSPLITDIALOG_H

#include <QString>

#include "gpx.h"

class QRadioButton;
class QCheckBox;

class GpxSplitDialog : public QDialog {
private:
    QRadioButton *bPreserve1;
    QRadioButton *bPreserve2;
    QCheckBox *bSave1;
    QCheckBox *bSave2;
    QString myFileName;
    GpxPoint myPoint;

    QString baseFileName() const;

public:
    GpxSplitDialog(const QString& fileName, const GpxPoint& point);
    bool isPreservedFirst() const;
    bool isSavedFirst() const;
    bool isSavedSecond() const;
    QString splitFileName(int part) const;
signals:

public slots:
};

#endif // GPXSPLITDIALOG_H
