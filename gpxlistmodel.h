#ifndef GPXLISTMODEL_H
#define GPXLISTMODEL_H

#include <QAbstractListModel>
#include "gpx.h"
#include "mapicon.h"

class GpxListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    const GpxPointList *myPoints;
    QList<int> myPoiList;
    const MapIconList *myMapIcons;
    void createPoiList();
public:
    enum {GpxIndexRole = Qt::UserRole+1};
    explicit GpxListModel(const GpxPointList *points, const MapIconList *mapIcons, QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
signals:

public slots:
    void updatePointList();
};

#endif // GPXLISTMODEL_H
