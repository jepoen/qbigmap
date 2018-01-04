#ifndef MANAGEWAYPOINTDLG_H
#define MANAGEWAYPOINTDLG_H

#include <QDialog>

#include "gpx.h"
#include "wptmodel.h"

class MapIconList;
class Model;
class WptModel;
class QAction;
class QTableView;

class ManageWayPointDlg : public QDialog
{
    Q_OBJECT
private:
    Model *myModel;
    WptModel *tableModel;
    const MapIconList *myMapIcons;
    QTableView *wptTable;
    QAction *moveUpAction;
    QAction *moveDownAction;
    QAction *editAction;

    void createActions();

public:
    explicit ManageWayPointDlg(Model *model, const MapIconList *mapIcons, QWidget *parent = 0);
    GpxPointList waypoints() const { return tableModel->waypoints(); }
    
signals:
private slots:
    void moveUp();
    void moveDown();
    void edit();
public slots:
    
};

#endif // MANAGEWAYPOINTDLG_H
