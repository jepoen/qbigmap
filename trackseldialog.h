#ifndef TRACKSELDIALOG_H
#define TRACKSELDIALOG_H

#include <QDialog>
#include <QTableView>
#include "track.h"
class QListWidget;

class TrackSelModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<SegInfo> mySegments;
public:
    TrackSelModel(QObject *parent = 0);
    TrackSelModel(const QList<SegInfo>& segments, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QSize size() const;
};

class TrackSelDialog : public QDialog
{
private:
    TrackSelModel model;
    QTableView *eSegments;
public:
    TrackSelDialog(QList<SegInfo> segments, QWidget *parent = 0);
    QList<int> selectedRows();
};

#endif // TRACKSELDIALOG_H
