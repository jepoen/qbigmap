#include <QtGui>
#include "trackseldialog.h"

TrackSelModel::TrackSelModel(QObject *parent) :
        QAbstractTableModel(parent)
{
}

TrackSelModel::TrackSelModel(const QList<TrackSegInfo> &segments, QObject *parent) :
        QAbstractTableModel(parent), mySegments(segments)
{}

int TrackSelModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return mySegments.size();
}

int TrackSelModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 4;
}

QVariant TrackSelModel::data(const QModelIndex &index, int role) const {
    int idx = index.row();
    if (role == Qt::DisplayRole) {
        TrackSegInfo seg = mySegments[idx];
        QDateTime lStart = seg.startTime().toLocalTime();
        QDateTime lStop = seg.endTime().toLocalTime();
        switch (index.column()) {
        case 0:
            return lStart.toString("yyyy-MM-dd");
        case 1:
            return lStart.toString("hh:mm:ss");
        case 2:
            return lStop.toString("hh:mm:ss");
        case 3:
            return seg.count();
        default:
            return QVariant();
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case 3:
            return QVariant(Qt::AlignRight|Qt::AlignVCenter);
        default:
            return QVariant(Qt::AlignLeft|Qt::AlignVCenter);
        }
    }
    return QVariant();
}

QVariant TrackSelModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Vertical)
        return section;
    if (role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Date");
        case 1:
            return tr("Start time");
        case 2:
            return tr("End time");
        case 3:
            return tr("Track points");
        }
    }
    else if (role == Qt::SizeHintRole) {
        switch (section) {
        case 0:
        case 1:
        case 2:
        case 3:
            return QSize(100, 25);
        default:
            return QSize(0, 0);
        }
    }
    return QVariant();
}

QSize TrackSelModel::size() const {
    int h = mySegments.size() >= 10 ? 300 : mySegments.size()*30+30;
    qDebug()<<"trackselmodel h "<<h;
    return QSize(450, h);
}

TrackSelDialog::TrackSelDialog(QList<TrackSegInfo> segments, QWidget *parent) :
        QDialog(parent), model(segments)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    eSegments = new QTableView();
    eSegments->setModel(&model);
    eSegments->setSelectionMode(QAbstractItemView::MultiSelection);
    eSegments->setSelectionBehavior(QAbstractItemView::SelectRows);
    //eSegments->resizeColumnsToContents();
    eSegments->horizontalHeader()->setStretchLastSection(true);
    eSegments->selectAll();
    mainLayout->addWidget(eSegments);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    //resize(model.size()+QSize(10, 60));
    eSegments->setMinimumSize(model.size());
    resizeTable();
    connect(&model, SIGNAL(layoutChanged()), this, SLOT(resizeTable()));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

QList<int> TrackSelDialog::selectedRows() {
    QList<int> rows;
    for (int i = 0; i < model.rowCount(QModelIndex()); i++) {
        QModelIndex index = model.index(i, 0);
        if (eSegments->selectionModel()->isSelected(index)) {
            rows.append(i);
        }
    }
    return rows;
}

void TrackSelDialog::resizeTable() {
    eSegments->resizeColumnsToContents();
}
