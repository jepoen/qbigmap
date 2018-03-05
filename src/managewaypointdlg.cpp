#include <QtWidgets>
#include "gpxpointdlg.h"
#include "managewaypointdlg.h"
#include "mapicon.h"
#include "mapview.h"
#include "model.h"
#include "wptmodel.h"

ManageWayPointDlg::ManageWayPointDlg(Model *model, MapView *mapView, const MapIconList *mapIcons, QWidget *parent) :
    QDialog(parent), myModel(model), myView(mapView), myMapIcons(mapIcons)
{
    tableModel = new WptModel(myModel->waypoints(), mapIcons, this);
    createActions();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *contentLayout = new QHBoxLayout();
    mainLayout->addLayout(contentLayout);
    wptTable = new QTableView();
    wptTable->setModel(tableModel);

    wptTable->resizeColumnsToContents();
    wptTable->setMinimumSize(wptTable->size());
    wptTable->horizontalHeader()->setStretchLastSection(true);
    wptTable->verticalHeader()->setVisible(false);
    wptTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    wptTable->setSelectionMode(QAbstractItemView::SingleSelection);
    wptTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    contentLayout->addWidget(wptTable);

    QVBoxLayout *controlLayout = new QVBoxLayout();
    QToolButton *bMoveUp = new QToolButton();
    bMoveUp->setDefaultAction(moveUpAction);
    controlLayout->addWidget(bMoveUp);
    QToolButton *bMoveDown = new QToolButton();
    bMoveDown->setDefaultAction(moveDownAction);
    controlLayout->addWidget(bMoveDown);
    QToolButton *bEdit = new QToolButton();
    bEdit->setDefaultAction(editAction);
    controlLayout->addWidget(bEdit);

    contentLayout->addLayout(controlLayout);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(wptTable, SIGNAL(activated(QModelIndex)), this, SLOT(changePos(QModelIndex)));
    connect(this, SIGNAL(posChanged(int)), myView, SLOT(changeWptPos(int)));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    if (myModel->waypoints().size() > 0) {
        //myView->createTempPoint(myModel->waypoints().at(0).coord());
        changePos(0);
        wptTable->selectRow(0);
    }
}

ManageWayPointDlg::~ManageWayPointDlg() {
    //myView->deleteTempPoint();
}

void ManageWayPointDlg::createActions() {
    moveUpAction = new QAction(QIcon(":/icons/arrow_up.png"), tr("Move up"), this);
    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveUp()));
    moveDownAction = new QAction(QIcon(":/icons/arrow_down.png"), tr("Move down"), this);
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveDown()));
    editAction = new QAction(QIcon(":/icons/pencil.png"), tr("Edit point"), this);
    connect(editAction, SIGNAL(triggered()), this, SLOT(edit()));
}

void ManageWayPointDlg::moveUp() {
    QModelIndexList indices = wptTable->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        tableModel->moveUp(index.row());
        wptTable->selectRow(index.row()-1);
    }
}

void ManageWayPointDlg::moveDown() {
    QModelIndexList indices = wptTable->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        tableModel->moveDown(index.row());
        wptTable->selectRow(index.row()+1);
    }
}

void ManageWayPointDlg::changePos(const QModelIndex &index) {
    int pos = index.row();
    changePos(pos);
}
void ManageWayPointDlg::changePos(int pos) {
    myModel->setWaypointPos(pos);
    //myView->changeWptPos(pos);
    //myView->moveTempPoint(myModel->waypoints().at(pos).coord());
}

void ManageWayPointDlg::edit() {
    QModelIndexList indices = wptTable->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        GpxPoint point = tableModel->waypoints().at(index.row());
        myView->createTempPoint(point.coord());
        GpxPointDlg dlg(myModel, point, *myMapIcons, false);
        if (dlg.exec() == QDialog::Accepted) {
            tableModel->update(index.row(), dlg.point());
        }
        myView->deleteTempPoint();
    }
}
