#ifndef PLACEDIALOG_H
#define PLACEDIALOG_H

#include <QAbstractTableModel>
#include <QDialog>
#include <QList>

class QDomElement;
class QLineEdit;
class QPushButton;
class QTableView;

class Place {
private:
    QPointF myPos;
    QString myName;
    QString myPlace;
    QString myIsIn;
public:
    explicit Place(const QPointF& pos) : myPos(pos) {}
    QPointF pos() const { return myPos; }
    QString name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    QString place() const { return myPlace; }
    void setPlace(const QString& place) { myPlace = place; }
    QString isIn() const { return myIsIn; }
    void setIsIn(const QString& isIn) { myIsIn = isIn; }
    void readTags(const QDomElement& parent);
};

class PlaceModel : public QAbstractTableModel {
private:
    QList<Place> myPlaces;
public:
    explicit PlaceModel(QObject *parent = 0) : QAbstractTableModel(parent) {}
    ~PlaceModel();
    int rowCount(const QModelIndex &/*parent*/) const { return myPlaces.size(); }
    int columnCount(const QModelIndex &/*parent*/) const { return 3; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setModel(const QList<Place> &places);
};

class PlaceDialog : public QDialog
{
    Q_OBJECT

private:
    QLineEdit *ePlace;
    QPushButton *bSearch;
    QList<Place> myPlaces;
    PlaceModel *myPlaceModel;
    QTableView *tPlaces;

public:
    const static QString overpassUrl;
    explicit PlaceDialog(QWidget *parent = 0);
    ~PlaceDialog();
    QPointF lonLat() const;

signals:
    
private slots:
    void download();
    void finish();
};

#endif // PLACEDIALOG_H
