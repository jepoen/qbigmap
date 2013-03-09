#ifndef TRACKEXPORTDLG_H
#define TRACKEXPORTDLG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class TrackExportDlg : public QDialog
{
    Q_OBJECT
private:
    QLabel *eTrkFile;
    QCheckBox *eHasWpts;
    QCheckBox *eOsm;
    QCheckBox *eSimple;
    QLineEdit *eTitle;
    QLabel *eOsmFile;
    QString myTrackFileName;

public:
    explicit TrackExportDlg(const QString& trackFileName, QWidget *parent = 0);
    QString title() const;
    QString fileName() const;
    QString osmFileName() const;
    bool isOsm() const;
    bool isSimple() const;
    void setWpts(bool val);
    bool hasWpts() const;
signals:

private slots:
    void selFileName();

public slots:
    
};

#endif // TRACKEXPORTDLG_H
