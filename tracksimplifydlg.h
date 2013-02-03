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
    QLineEdit *eTitle;
    QLabel *eOsmFile;
    QPolygonF myLine;
    QList<bool> myStopNodes;
    QPointF myCenter;

    void initLine();
    QString getSimpleFileName(const QString& s);
    void redrawTrack();

public:
    TrackSimplifyDlg(MapScene *scene, QWidget *parent = 0);
    QString fileName() const;
    QString osmFileName() const;
    QString title() const;
    Track *simpleTrack() const { return mySimpleTrack.get(); }
    bool isOsm() const;
    bool writeXml() const;
private slots:
    void selFileName();
    void simplify(int val);
    void finish();

};

#endif // TRACKSIMPLIFYDLG_H
