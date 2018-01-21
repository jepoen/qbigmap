#include <QtDebug>
#include <QtWidgets>
#include <QtNetwork>
#include <QtXml>
#include "placedialog.h"

const QString PlaceDialog::overpassUrl("http://overpass-api.de/api/interpreter");

void Place::readTags(const QDomElement& parent) {
    for (QDomElement el = parent.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
        if (el.nodeName() != "tag") continue;
        qDebug()<<el.attribute("k")<<": "<<el.attribute("v");
        QString key = el.attribute("k");
        if (key == "place") myPlace = el.attribute("v");
        else if (key == "name") myName = el.attribute("v");
        else if (key == "is_in") myIsIn = el.attribute("v");
    }
}

PlaceModel::~PlaceModel() {
    qDebug()<<"PlaceModel destroyed";
}

void PlaceModel::setModel(const QList<Place> &places) {
    beginResetModel();
    myPlaces = places;
    endResetModel();
}

QVariant PlaceModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Vertical) return section+1;
    else {
        switch (section) {
        case 0: return tr("Place");
        case 1: return tr("Longitude");
        case 2: return tr("Latitude");
        }
        return QVariant();
    }
}

QVariant PlaceModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        const Place& pl = myPlaces[index.row()];
        switch(index.column()) {
        case 0:
            return tr("%1 (%2, is in: %3)").arg(pl.name()).arg(pl.place()).arg(pl.isIn());
        case 1:
            return pl.pos().x();
        case 2:
            return pl.pos().y();
        }
    }
    return QVariant();
}

PlaceDialog::PlaceDialog(QWidget *parent) :
    QDialog(parent)
{
    myPlaceModel = new PlaceModel();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lPlace = new QLabel(tr("Place:"));
    ePlace = new QLineEdit();
    bSearch = new QPushButton(tr("Search"));
    bSearch->setDefault(true);
    control->addWidget(lPlace, 0, 0);
    control->addWidget(ePlace, 0, 1);
    control->addWidget(bSearch, 0, 2);
    mainLayout->addLayout(control);
    tPlaces = new QTableView();
    tPlaces->setModel(myPlaceModel);
    tPlaces->setMinimumSize(400, 200);
    tPlaces->setSelectionMode(QAbstractItemView::SingleSelection);
    tPlaces->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView *columns = tPlaces->horizontalHeader();
    columns->setSectionResizeMode(0, QHeaderView::Stretch);
    mainLayout->addWidget(tPlaces);
    QDialogButtonBox *box = new QDialogButtonBox();
    box->addButton(tr("Select"), QDialogButtonBox::AcceptRole);
    box->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(bSearch, SIGNAL(clicked()), this, SLOT(download()));
    connect(box, SIGNAL(accepted()), this, SLOT(finish()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

PlaceDialog::~PlaceDialog() {
    delete myPlaceModel;
}

void PlaceDialog::finish() {
    if (tPlaces->currentIndex().isValid()) emit accept();
}

QPointF PlaceDialog::lonLat() const {
    int idx = tPlaces->currentIndex().row();
    return myPlaces[idx].pos();
}

void PlaceDialog::download() {
    if (ePlace->text().trimmed().isEmpty()) {
        myPlaceModel->setModel(QList<Place>());
        return;
    }
    QUrl url(overpassUrl);
    QUrlQuery postData;
    postData.addQueryItem("data", QString("node[place][name=\"%1\"];out qt;").arg(ePlace->text().trimmed()));
    qDebug()<<url<<" "<<postData.toString();
    QProgressDialog *progressDlg = new QProgressDialog(this);
    progressDlg->setLabelText(tr("Download %1").arg(url.toString()));
    QNetworkAccessManager netManager(this);
    QNetworkReply *reply = netManager.post(QNetworkRequest(QUrl(url)), postData.toString(QUrl::FullyEncoded).toUtf8());
    QEventLoop pause;
    connect(reply, SIGNAL(finished()), &pause, SLOT(quit()));
    //connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    //connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgress(qint64, qint64)));
    progressDlg->show();
    pause.exec();
    progressDlg->hide();
    qDebug()<<"download complete";
    //qDebug()<<reply->readAll();
    myPlaces.clear();
    QDomDocument doc;
    QString errorStr;
    if (!doc.setContent(reply->readAll(), false, &errorStr)) {
        qDebug()<<errorStr;
        return;
    }
    QDomElement root = doc.documentElement();
    for (QDomElement el = root.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
        if (el.nodeName() != "node") continue;
        double lon = el.attribute("lon").toDouble();
        double lat = el.attribute("lat").toDouble();
        qDebug()<<el.nodeName()<<"("<<lon<<","<<lat<<")";
        Place place(QPointF(lon, lat));
        place.readTags(el);
        myPlaces.push_back(place);
    }
    myPlaceModel->setModel(myPlaces);
}

