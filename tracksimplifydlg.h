#ifndef TRACKSIMPLIFYDLG_H
#define TRACKSIMPLIFYDLG_H
#include <memory>
#include <QDialog>
#include <QPolygonF>

class MapScene;
class QAction;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
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
    QPushButton *bExport;
    QPushButton *bReplace;
    QPolygonF myLine;
    QList<bool> myStopNodes;
    QPointF myCenter;
    int myAction;

    void initLine();
    QString getSimpleFileName(const QString& s);
    void redrawTrack();

public:
    enum {CANCEL, EXPORT, REPLACE};
    TrackSimplifyDlg(MapScene *scene, QWidget *parent = 0);
    ~TrackSimplifyDlg();
    QString fileName() const;
    QString osmFileName() const;
    Track *simpleTrack() const { return mySimpleTrack.get(); }
    bool isOsm() const;
    bool writeXml() const;
    int action() const { return myAction; }
private slots:
    void selFileName();
    void simplify(int val);
    void finish();
    void exportTrk();
    void replaceTrk();
};

#endif // TRACKSIMPLIFYDLG_H
