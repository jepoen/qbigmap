#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include "model.h"

class LayersModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<Layer> *myLayers;

public:
    LayersModel(QObject *parent = 0);
    LayersModel(QList<Layer> *layers, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void moveUp(int row);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QList<Layer> *layers() { return myLayers; }
};

#endif // LAYERSMODEL_H
