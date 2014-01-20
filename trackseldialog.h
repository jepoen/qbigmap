#ifndef TRACKSELDIALOG_H
#define TRACKSELDIALOG_H

#include <QDialog>
#include <QTableView>
#include "gpx.h"
class QListWidget;

class TrackSelModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<TrackSegInfo> mySegments;
public:
    TrackSelModel(QObject *parent = 0);
    TrackSelModel(const QList<TrackSegInfo>& segments, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QSize size() const;
};

class TrackSelDialog : public QDialog
{
    Q_OBJECT
private:
    TrackSelModel model;
    QTableView *eSegments;
public:
    TrackSelDialog(QList<TrackSegInfo> segments, QWidget *parent = 0);
    QList<int> selectedRows();
private slots:
    void resizeTable();
};

#endif // TRACKSELDIALOG_H
