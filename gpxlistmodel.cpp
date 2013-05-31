#include <QtDebug>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include "boolspinbox.h"
#include "gpxlistmodel.h"
#include "model.h"

GpxListModel::GpxListModel(Model *model, const MapIconList *mapIcons, QObject *parent) :
    QAbstractTableModel(parent), myModel(model), myMapIcons(mapIcons)
{
    createPoiList();
}

void GpxListModel::createPoiList() {
    QModelIndex parent;
    beginRemoveRows(parent, 0, myPoiList.size()-1);
    myPoiList.clear();
    endRemoveRows();
    const GpxPointList& points = myModel->track().trackPoints();
    for (int i = 0; i < points.size(); i++) {
        if (!points[i].sym().isEmpty()) {
            myPoiList.append(i);
            qDebug()<<" poi "<<i<<" "<<points[i].sym();
        }
    }
    beginInsertRows(parent, 0, myPoiList.size()-1);
    endInsertRows();
}

QVariant GpxListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case 0:
        return tr("POI");
    case 1:
        return tr("Map");
    case 2:
        return tr("Profile");
    }
    return QVariant();
}

QVariant GpxListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    int row = index.row();
    int idx = myPoiList[row];
    const GpxPointList& points = myModel->track().trackPoints();
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return points[idx].name();
        case 1:
            return points[idx].showMap();
        case 2:
            return points[idx].showProfile();
        default:
            return QVariant();
        }
    } else if (role == Qt::DecorationRole) {
        QString key = points[idx].sym();
        switch (index.column()) {
        case 0:
            return myMapIcons->icon(key).ico();
        }
    } else if (role == GpxIndexRole) {
        return idx;
    } else if (role == Qt::EditRole) {
        switch(index.column()) {
        case 0:
            return QVariant();
        case 1:
            return points[idx].showMap();
        case 2:
            return points[idx].showProfile();
        }
    }
    return QVariant();
}

int GpxListModel::rowCount(const QModelIndex &parent) const {
    return myModel->track().trackPoints().size();
}

void GpxListModel::updatePointList() {
    createPoiList();
    //QModelIndex idx0 = createIndex(0, 0);
    //QModelIndex idx1 = createIndex(myPoiList.size(), 0);
    //emit dataChanged(idx0, idx1);
}

Qt::ItemFlags GpxListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    int col = index.column();
    if (col == 0) return QAbstractTableModel::flags(index);
    else          return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool GpxListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole) return false;
    int row = index.row();
    int idx = myPoiList[row];
    qDebug()<<"Update track point "<<idx<<" "<<index.column()<<": "<<value<<" "<<value.toBool();
    qDebug()<<"old values "<<myModel->track().trackPoint(idx).showMap()<<" "<<myModel->track().trackPoint(idx).showProfile();
    GpxPoint p = myModel->track().trackPoint(idx);
    switch (index.column()) {
    case 1:
        p.setShowMap(value.toBool());
        myModel->setTrackPoint(idx, p);
        break;
    case 2:
        p.setShowProfile(value.toBool());
        myModel->setTrackPoint(idx, p);
        break;
    }
    emit dataChanged(index, index);
    return true;
}

GpxListDelegate::GpxListDelegate(QObject *parent) :
    QItemDelegate(parent)
{}

QSize GpxListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 0) return QItemDelegate::sizeHint(option, index);
    else return QSize(16, 16);
}

void GpxListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QPixmap pix;
    switch (index.column()) {
    case 1:
    case 2:
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        pix = index.data().toBool() ? QPixmap(":/icons/add.png") : QPixmap(":/icons/delete.png");
        painter->drawPixmap(option.rect.x()+option.rect.width()/2-8, option.rect.y()+option.rect.height()/2-8, 16, 16, pix);
        return;
    default:
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget* GpxListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 1 || index.column() == 2) {
        BoolSpinBox *box = new BoolSpinBox(parent);
        connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndClose()));
        return box;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void GpxListDelegate::commitAndClose() {
    BoolSpinBox *box = qobject_cast<BoolSpinBox*>(sender());
    emit commitData(box);
    emit closeEditor(box);
}

void GpxListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() == 1 || index.column() == 2) {
        BoolSpinBox *box = static_cast<BoolSpinBox*>(editor);
        box->setCurrentIndex(index.model()->data(index, Qt::EditRole).toInt());
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void GpxListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (index.column() == 1 || index.column() == 2) {
        BoolSpinBox *box = new BoolSpinBox(editor);
        model->setData(index, box->itemData(box->currentIndex()));
    }
    QItemDelegate::setModelData(editor, model, index);
}
