#include "mapicon.h"
#include "wptmodel.h"

int WptModel::rowCount(const QModelIndex &/*parent*/) const {
    return myWayPoints.size();
}

int WptModel::columnCount(const QModelIndex &/*parent*/) const {
    return 2;
}

QVariant WptModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    if (index.row() >= myWayPoints.size() || index.row() < 0) return QVariant();
    const GpxPoint point = myWayPoints.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return point.sym();
        } else if (index.column() == 1) {
            return point.name()+"\n"+point.cmt();
        }
    } else if (role == Qt::DecorationRole) {
        if (index.column() == 0) {
            int idx = myMapIcons->iconIdx(point.sym());
            if (idx >= 0) {
                return myMapIcons->at(idx).ico();
            }
        }
    } else if (role == Qt::ToolTipRole) {
        return point.cmt();
    }
    return QVariant();
}

QVariant WptModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        if (section == 0) return tr("Symbol");
        else if (section == 1) return tr("Name, Comment");
    } else {
        return section;
    }
    return QVariant();
}

void WptModel::moveUp(int row) {
    if (row <= 0) return;
    GpxPoint pt = myWayPoints.at(row);
    myWayPoints.removeAt(row);
    myWayPoints.insert(row-1, pt);
    QModelIndex idx0 = index(row-1, 0, QModelIndex());
    QModelIndex idx1 = index(row, 1, QModelIndex());
    emit dataChanged(idx0, idx1);
}

void WptModel::moveDown(int row) {
    if (row >= myWayPoints.size()-1) return;
    GpxPoint pt = myWayPoints.at(row);
    myWayPoints.removeAt(row);
    myWayPoints.insert(row+1, pt);
    QModelIndex idx0 = index(row, 0, QModelIndex());
    QModelIndex idx1 = index(row+1, 1, QModelIndex());
    emit dataChanged(idx0, idx1);
}

void WptModel::update(int row, const GpxPoint &point) {
    myWayPoints[row] = point;
    QModelIndex idx0 = index(row, 0);
    QModelIndex idx1 = index(row, 1);
    emit dataChanged(idx0, idx1);
}
