#ifndef LAYERSDIALOG_H
#define LAYERSDIALOG_H

#include <QDialog>
#include <QList>
#include "model.h"
#include "layersmodel.h"

class QAction;
class QTableView;

class LayersDialog : public QDialog
{
    Q_OBJECT
private:
    QList<Layer> myLayers;
    LayersModel *model;
    QTableView *layerTab;
    QAction *addEntryAction;
    QAction *delEntryAction;
    QAction *moveUpAction;
    QAction *moveDownAction;
    void createActions();

public:
    LayersDialog(const QList<Layer>& layers, QWidget *parent = 0);
    const QList<Layer>& layers() const { return myLayers; }
private slots:
    void addEntry();
    void delEntry();
    void editEntry();
    void moveUp();
    void moveDown();
};

#endif // LAYERSDIALOG_H
