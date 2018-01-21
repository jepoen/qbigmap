#ifndef TRACKEXPORTDLG_H
#define TRACKEXPORTDLG_H

#include <QDialog>

class QAction;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class TrackExportDlg : public QDialog
{
    Q_OBJECT
private:
    QAction *exportDirAction;
    QLabel *eExportDir;
    QLabel *eTrkFile;
    QCheckBox *eHasWpts;
    QCheckBox *eOsm;
    QCheckBox *eSimple;
    QLineEdit *eTitle;
    QLabel *eOsmFile;
    QString myExportDir;
    QString myTrackFileName;

public:
    explicit TrackExportDlg(const QString& exportDir, const QString& trackFileName, QWidget *parent = 0);
    QString title() const;
    QString fileName() const;
    QString exportDir() const { return myExportDir; }
    QString osmFileName() const;
    bool isOsm() const;
    bool isSimple() const;
    void setWpts(bool val);
    bool hasWpts() const;
signals:

private slots:
    void selExportDir();

public slots:
    
};

#endif // TRACKEXPORTDLG_H
