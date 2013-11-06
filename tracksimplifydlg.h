#ifndef TRACKSIMPLIFYDLG_H
#define TRACKSIMPLIFYDLG_H
#include <QDialog>
#include <QPolygonF>
#include <track.h>

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
    //std::auto_ptr<Track> mySimpleTrack;
    TrackItem *myTrackItem;
    QSpinBox *eFailure;
    QLabel *eTrackPoints;
    QPushButton *bExport;
    QPushButton *bReplace;
    QPolygonF myLine;
    QList<GpxPoint> mySimpleTrackPoints;
    QList<bool> myStopNodes;
    QPointF myCenter;
    int myAction;

    void initLine();
    QString getSimpleFileName(const QString& s);
    void redrawTrack();
    void finish();

public:
    enum {CANCEL, EXPORT, REPLACE};
    TrackSimplifyDlg(MapScene *scene, QWidget *parent = 0);
    ~TrackSimplifyDlg();
    QList<GpxPoint> simpleTrackPoints() const { return mySimpleTrackPoints; }
    int action() const { return myAction; }

private slots:
    void simplify(int val);
    void exportTrk();
    void replaceTrk();
    void cancel();
};

#endif // TRACKSIMPLIFYDLG_H
