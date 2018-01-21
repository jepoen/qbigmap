#ifndef WPTMODEL_H
#define WPTMODEL_H

#include <QAbstractTableModel>
#include "gpx.h"

class MapIconList;

class WptModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    GpxPointList myWayPoints;
    const MapIconList *myMapIcons;

public:
    explicit WptModel(const GpxPointList& wayPoints, const MapIconList *mapIcons, QObject *parent = 0):
        QAbstractTableModel(parent), myWayPoints(wayPoints), myMapIcons(mapIcons)
    {}

    int rowCount(const QModelIndex &) const;

    int columnCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void moveUp(int row);
    void moveDown(int row);
    void update(int row, const GpxPoint& point);
    GpxPointList waypoints() const { return myWayPoints; }

signals:
    
public slots:
    
};

#endif // WPTMODEL_H
