#ifndef TRACKSIMPLIFYDLG_H
#define TRACKSIMPLIFYDLG_H
#include <memory>
#include <QDialog>

class MapScene;
class QAction;
class QCheckBox;
class QLabel;
class QSpinBox;
class Track;
class TrackItem;

class TrackSimplifyDlg : public QDialog
{
        Q_OBJECT
private:
    MapScene *myScene;
    std::auto_ptr<Track> mySimpleTrack;
    std::auto_ptr<TrackItem> myTrackItem;
    QAction *selFileAction;
    QSpinBox *eFailure;
    QLabel *eTrackPoints;
    QLabel *eFileName;
    QCheckBox *eOsm;
    QCheckBox *eGoogle;
    QString getSimpleFileName(const QString& s);
    void redrawTrack();

public:
    TrackSimplifyDlg(MapScene *scene, QWidget *parent = 0);
    QString fileName() const;
    bool isOsm() const;
    bool isGoogle() const;
    bool writeXml() const;
private slots:
    void selFileName();
    void simplify(int val);

};

#endif // TRACKSIMPLIFYDLG_H
