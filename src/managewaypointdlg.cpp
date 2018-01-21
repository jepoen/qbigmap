#include <QtWidgets>
#include "gpxpointdlg.h"
#include "managewaypointdlg.h"
#include "mapicon.h"
#include "model.h"
#include "wptmodel.h"

ManageWayPointDlg::ManageWayPointDlg(Model *model, const MapIconList *mapIcons, QWidget *parent) :
    QDialog(parent), myModel(model), myMapIcons(mapIcons)
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
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
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

void ManageWayPointDlg::edit() {
    QModelIndexList indices = wptTable->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        GpxPoint point = tableModel->waypoints().at(index.row());
        GpxPointDlg dlg(myModel, point, *myMapIcons, false);
        if (dlg.exec() == QDialog::Accepted) {
            tableModel->update(index.row(), dlg.point());
        }
    }
}
