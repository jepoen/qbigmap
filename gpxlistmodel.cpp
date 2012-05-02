#include <QtDebug>
#include "gpxlistmodel.h"

GpxListModel::GpxListModel(const GpxPointList *points, const MapIconList *mapIcons, QObject *parent) :
    QAbstractListModel(parent), myPoints(points), myMapIcons(mapIcons)
{
    createPoiList();
}

void GpxListModel::createPoiList() {
    QModelIndex parent;
    beginRemoveRows(parent, 0, myPoiList.size()-1);
    myPoiList.clear();
    endRemoveRows();

    for (int i = 0; i < myPoints->size(); i++) {
        if (!myPoints->at(i).sym().isEmpty()) {
            myPoiList.append(i);
            qDebug()<<" poi "<<i<<" "<<myPoints->at(i).sym();
        }
    }
    beginInsertRows(parent, 0, myPoiList.size()-1);
    endInsertRows();
}

QVariant GpxListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    //if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return tr("POI");
    //}
    return QVariant();
}

QVariant GpxListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    int row = index.row();
    int idx = myPoiList[row];
    if (role == Qt::DisplayRole) {
        return myPoints->at(idx).name();
    } else if (role == Qt::DecorationRole) {
        QString key = myPoints->at(idx).sym();
        return myMapIcons->icon(key).ico();
    } else if (role == GpxIndexRole) {
        return idx;
    }
    return QVariant();
}

int GpxListModel::rowCount(const QModelIndex &parent) const {
    return myPoiList.size();
}

void GpxListModel::updatePointList() {
    createPoiList();
    //QModelIndex idx0 = createIndex(0, 0);
    //QModelIndex idx1 = createIndex(myPoiList.size(), 0);
    //emit dataChanged(idx0, idx1);
}
