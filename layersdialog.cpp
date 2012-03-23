#include <QtGui>
#include "layersdialog.h"

LayersDialog::LayersDialog(const QList<Layer>& layers, QWidget *parent) :
        QDialog(parent),
        myLayers(layers)
{
    createActions();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *content = new QHBoxLayout();
    model = new LayersModel(&myLayers);
    layerTab = new QTableView();
    layerTab->setModel(model);
    layerTab->resizeColumnsToContents();
    layerTab->setMinimumSize(layerTab->size());
    layerTab->horizontalHeader()->setStretchLastSection(true);
    layerTab->setSelectionMode(QAbstractItemView::SingleSelection);
    content->addWidget(layerTab, 1);
    QVBoxLayout *controls = new QVBoxLayout();
    QToolButton *bAdd = new QToolButton();
    bAdd->setDefaultAction(addEntryAction);
    controls->addWidget(bAdd);
    QToolButton *bDel = new QToolButton();
    bDel->setDefaultAction(delEntryAction);
    controls->addWidget(bDel);
    QToolButton *bUp = new QToolButton();
    bUp->setDefaultAction(moveUpAction);
    controls->addWidget(bUp);
    content->addLayout(controls);
    mainLayout->addLayout(content);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    mainLayout->update();
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    QModelIndex index = model->index(0, 0, QModelIndex());
    layerTab->setCurrentIndex(index);
}

void LayersDialog::createActions() {
    addEntryAction = new QAction(QIcon(":/icons/add.png"), tr("Add entry"), this);
    delEntryAction = new QAction(QIcon(":/icons/delete.png"), tr("Delete entry"), this);
    moveUpAction = new QAction(QIcon(":/icons/arrow_up.png"), tr("Move up"), this);
    connect(addEntryAction, SIGNAL(triggered()), this, SLOT(addEntry()));
    connect(delEntryAction, SIGNAL(triggered()), this, SLOT(delEntry()));
    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveUp()));
}

void LayersDialog::addEntry() {
    model->insertRow(myLayers.size());
    QModelIndex index = model->index(myLayers.size()-1, 0, QModelIndex());
    layerTab->setCurrentIndex(index);
    //layerTab->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    layerTab->scrollTo(index);
    layerTab->edit(index);
}

void LayersDialog::delEntry() {
    QModelIndexList indices = layerTab->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        model->removeRow(index.row());
    }
}

void LayersDialog::editEntry() {

}

void LayersDialog::moveUp() {
    QModelIndexList indices = layerTab->selectionModel()->selectedIndexes();
    if (indices.size() > 0) {
        QModelIndex index = indices[0];
        model->moveUp(index.row());
        layerTab->selectRow(index.row()-1);
    }
}

void LayersDialog::moveDown() {

}
