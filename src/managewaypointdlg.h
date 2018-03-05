#ifndef MANAGEWAYPOINTDLG_H
#define MANAGEWAYPOINTDLG_H

#include <QDialog>
#include <QModelIndex>

#include "gpx.h"
#include "wptmodel.h"

class MapIconList;
class MapView;
class Model;
class WptModel;
class QAction;
class QTableView;

class ManageWayPointDlg : public QDialog
{
    Q_OBJECT
private:
    Model *myModel;
    MapView *myView;
    WptModel *tableModel;
    const MapIconList *myMapIcons;
    QTableView *wptTable;
    QAction *moveUpAction;
    QAction *moveDownAction;
    QAction *editAction;

    void createActions();
    void changePos(int pos);

public:
    explicit ManageWayPointDlg(Model *model, MapView *mapView, const MapIconList *mapIcons, QWidget *parent = 0);
    ~ManageWayPointDlg();
    GpxPointList waypoints() const { return tableModel->waypoints(); }
    
signals:
    void posChanged(int);

private slots:
    void changePos(const QModelIndex&);
    void moveUp();
    void moveDown();
    void edit();
public slots:
    
};

#endif // MANAGEWAYPOINTDLG_H
