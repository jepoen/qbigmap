#ifndef DELTRKPARTDLG_H
#define DELTRKPARTDLG_H

#include <QDialog>

class Track;
class MapScene;
class Model;
class QSpinBox;
class QGraphicsLineItem;

class DelTrkPartDlg : public QDialog
{
    Q_OBJECT
private:
    MapScene *myScene;
    Model *myModel;
    Track *myTrack;
    QSpinBox *eFrom;
    QSpinBox *eTo;
    QGraphicsLineItem *myItem;

    void updateItem(int i0, int i1);
public:
    explicit DelTrkPartDlg(MapScene *scene, QWidget *parent = 0);
    ~DelTrkPartDlg();
    int pos0() const;
    int pos1() const;
signals:
    
private slots:
    void setFrom(int pos);
    void setTo(int pos);
public slots:
    
};

#endif // DELTRKPARTDLG_H
