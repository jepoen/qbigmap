#include <QtGui>
#include <QtDebug>
#include "layersmodel.h"

LayersModel::LayersModel(QObject *parent) :
        QAbstractTableModel(parent)
{}

LayersModel::LayersModel(QList<Layer> *layers, QObject *parent) :
        QAbstractTableModel(parent),
        myLayers(layers)
{}

int LayersModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return myLayers->size();
}

int LayersModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant LayersModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    if (index.row() >= myLayers->size() || index.row() < 0) return QVariant();
    if (role == Qt::DisplayRole) {
        Layer ly = myLayers->at(index.row());
        if (index.column() == 0)
            return ly.name();
        else if (index.column() == 1)
            return ly.url();
    }
    else if (role == Qt::EditRole) {
        qDebug()<<"editRole";
        Layer ly = myLayers->at(index.row());
        if (index.column() == 0)
            return ly.name();
        else if (index.column() == 1)
            return ly.url();
    }
    return QVariant();
}

QVariant LayersModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("URL");
        default:
            return QVariant();
        }
    }
    else if (orientation == Qt::Vertical) {
        return section;
    }
    return QVariant();
}

bool LayersModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;
    Layer ly = myLayers->at(index.row());
    if (index.column() == 0)
        ly.setName(value.toString());
    else if (index.column() == 1)
        ly.setUrl(value.toString());
    myLayers->replace(index.row(), ly);
    return true;
}

Qt::ItemFlags LayersModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


bool LayersModel::insertRows(int position, int rows, const QModelIndex &parent) {
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; row++) {
        Layer ly(QString(""), QString(""));
        myLayers->insert(position, ly);
    }
    endInsertRows();
    return true;
}

bool LayersModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for (int i = 0; i < count; i++) {
        myLayers->removeAt(row);
    }
    endRemoveRows();
    return true;
}

void LayersModel::moveUp(int row) {
    if (row <= 0)
        return;
    Layer ly = myLayers->at(row);
    myLayers->removeAt(row);
    myLayers->insert(row-1, ly);
    QModelIndex idx0 = index(row-1, 0, QModelIndex());
    QModelIndex idx1 = index(row, 1, QModelIndex());
    emit dataChanged(idx0, idx1);
}
