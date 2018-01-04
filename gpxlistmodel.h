#ifndef GPXLISTMODEL_H
#define GPXLISTMODEL_H

#include <QAbstractTableModel>
#include <QItemDelegate>
#include "gpx.h"
#include "mapicon.h"

class Model;

class GpxListModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    Model *myModel;
    QList<int> myPoiList;
    const MapIconList *myMapIcons;
    void createPoiList();
public:
    enum {GpxIndexRole = Qt::UserRole+1};
    explicit GpxListModel(Model *model, const MapIconList *mapIcons, QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &/*parent*/) const { return 3; }
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
signals:

public slots:
    void updatePointList();
};

class GpxListDelegate : public QItemDelegate {
    Q_OBJECT
public:
    explicit GpxListDelegate(QObject *parent = 0);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private slots:
    void commitAndClose();
};

#endif // GPXLISTMODEL_H
