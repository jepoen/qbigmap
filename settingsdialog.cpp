#include <QtGui>
#include <QtDebug>
#include "settingsdialog.h"

GpsTab::GpsTab(Settings *settings, QWidget *parent) :
        QWidget(parent), mySettings(settings)
{
    QGridLayout *control = new QGridLayout();
    QLabel *lGpsBabel = new QLabel(tr("Path of gpsbabel"));
    control->addWidget(lGpsBabel, 0, 0);
    eGpsBabel = new QLineEdit(mySettings->gpsbabel());
    control->addWidget(eGpsBabel, 0, 1);
    bGpsBabel = new QPushButton(tr("Select"));
    control->addWidget(bGpsBabel, 0, 2);
    QLabel *lGpsDevice = new QLabel(tr("GPS device (for gpsbabel):"));
    control->addWidget(lGpsDevice, 1, 0);
    eGpsDevice = new QLineEdit(mySettings->gpsDevice());
    control->addWidget(eGpsDevice, 1, 1);
    QLabel *lGpsInterface = new QLabel(tr("GPS interface (for gpsbabel):"));
    control->addWidget(lGpsInterface, 2, 0);
    eGpsInterface = new QLineEdit(mySettings->gpsInterface());
    control->addWidget(eGpsInterface, 2, 1);

    setLayout(control);
    connect(bGpsBabel, SIGNAL(clicked()), this, SLOT(selectGpsBabel()));
}

void GpsTab::selectGpsBabel() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select gpsbabel"), "/usr/bin", "gpsbabel");
    if (!fileName.isEmpty()) {
        eGpsBabel->setText(fileName);
    }
}

const QString GpsTab::gpsbabel() const {
    return eGpsBabel->text();
}
const QString GpsTab::gpsDevice() const {
    return eGpsDevice->text();
}
const QString GpsTab::gpsInterface() const {
    return eGpsInterface->text();
}

DirTab::DirTab(Settings *settings, QWidget *parent) :
        QWidget(parent), mySettings(settings)
{
    QGridLayout *control = new QGridLayout();
    QLabel *lTrackDir = new QLabel(tr("&Track directory:"));
    eTrackDir = new QLineEdit(mySettings->trackDir());
    lTrackDir->setBuddy(eTrackDir);
    bTrackDir = new QPushButton(tr("Select"));
    control->addWidget(lTrackDir, 0, 0);
    control->addWidget(eTrackDir, 0, 1);
    control->addWidget(bTrackDir, 0, 2);
    QLabel *lSrtmDir = new QLabel(tr("SRTM data directory:"));
    eSrtmDir = new QLineEdit(mySettings->srtmDir());
    lSrtmDir->setBuddy(eSrtmDir);
    bSrtmDir = new QPushButton(tr("Select"));
    control->addWidget(lSrtmDir, 1, 0);
    control->addWidget(eSrtmDir, 1, 1);
    control->addWidget(bSrtmDir, 1, 2);
    setLayout(control);
    connect(bTrackDir, SIGNAL(clicked()), this, SLOT(selectTrackDir()));
    connect(bSrtmDir, SIGNAL(clicked()), this, SLOT(selectSrtmDir()));
}

void DirTab::selectTrackDir() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select track dir"), mySettings->trackDir());
    if (!dirName.isEmpty()) {
        eTrackDir->setText(dirName);
    }
}

void DirTab::selectSrtmDir() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select SRTM dir"), mySettings->srtmDir());
    if (!dirName.isEmpty()) {
        eSrtmDir->setText(dirName);
    }
}

const QString DirTab::trackDir() const {
    return eTrackDir->text();
}

const QString DirTab::srtmDir() const {
    return eSrtmDir->text();
}

PrintTab::PrintTab(Settings *settings, QWidget *parent) :
        QWidget(parent), mySettings(settings)
{
    QGridLayout *control = new QGridLayout();
    QLabel *lTileSize = new QLabel(tr("&Tile size (mm):"));
    control->addWidget(lTileSize, 0, 0);
    bTileSize = new QSpinBox();
    bTileSize->setRange(5, 200);
    bTileSize->setValue(mySettings->tileSize());
    lTileSize->setBuddy(bTileSize);
    control->addWidget(bTileSize, 0, 1);
    QLabel *lTrackWidth = new QLabel(tr("Track &width (pt):"));
    control->addWidget(lTrackWidth, 1, 0);
    bTrackWidth = new QSpinBox();
    bTrackWidth->setRange(0, 20);
    bTrackWidth->setValue(mySettings->outTrackWidth());
    lTrackWidth->setBuddy(bTrackWidth);
    control->addWidget(bTrackWidth, 1, 1);
    QLabel *lTrackColor = new QLabel(tr("Track &color:"));
    control->addWidget(lTrackColor, 2, 0);
    myTrackColor = mySettings->outTrackColor();
    bTrackColor = new QPushButton(SettingsDialog::colorIcon(myTrackColor), tr("Select"));
    lTrackColor->setBuddy(bTrackColor);
    control->addWidget(bTrackColor, 2, 1);
    QLabel *lRouteWidth = new QLabel(tr("Route width (pt):"));
    control->addWidget(lRouteWidth, 3, 0);
    bRouteWidth = new QSpinBox();
    bRouteWidth->setRange(0, 20);
    bRouteWidth->setValue(mySettings->outRouteWidth());
    lRouteWidth->setBuddy(bRouteWidth);
    control->addWidget(bRouteWidth, 3, 1);
    QLabel *lRouteColor = new QLabel(tr("Track &color:"));
    control->addWidget(lRouteColor, 4, 0);
    myRouteColor = mySettings->outRouteColor();
    bRouteColor = new QPushButton(SettingsDialog::colorIcon(myRouteColor), tr("Select"));
    lRouteColor->setBuddy(bRouteColor);
    control->addWidget(bRouteColor, 4, 1);
    setLayout(control);
    connect(bTrackColor, SIGNAL(clicked()), this, SLOT(selTrackColor()));
    connect(bRouteColor, SIGNAL(clicked()), this, SLOT(selRouteColor()));
}

int PrintTab::tileSize() const {
    return bTileSize->value();
}

int PrintTab::trackWidth() const {
    return bTrackWidth->value();
}

QColor PrintTab::trackColor() const {
    return myTrackColor;
}

QColor PrintTab::routeColor() const {
    return myRouteColor;
}

void PrintTab::selTrackColor() {
    QColor res = QColorDialog::getColor(myTrackColor, this, tr("Select Track Color"),
                                        QColorDialog::ShowAlphaChannel);
    if (res.isValid()) {
        myTrackColor = res;
        bTrackColor->setIcon(SettingsDialog::colorIcon(myTrackColor));
    }
}

void PrintTab::selRouteColor() {
    QColor res = QColorDialog::getColor(myRouteColor, this, tr("Select Route Color"),
                                        QColorDialog::ShowAlphaChannel);
    if (res.isValid()) {
        myRouteColor = res;
        bRouteColor->setIcon(SettingsDialog::colorIcon(myRouteColor));
    }
}

IconTableModel::IconTableModel(const QList<MapIcon> &icons, QObject *parent) :
    QAbstractTableModel(parent), myIcons(icons)
{

}

int IconTableModel::rowCount(const QModelIndex &parent) const {
    return myIcons.size();
}

int IconTableModel::columnCount(const QModelIndex &parent) const {
    return 3;
}

QVariant IconTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    int idx = index.row();
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0:
            return myIcons[idx].name();
        case 1:
            return myIcons[idx].icoFile();
        case 2:
            return myIcons[idx].mapIcoFile();
        }
    } else if (role == Qt::DecorationRole) {
        switch(index.column()) {
        case 1:
            return myIcons[idx].ico();
        }
    }
    return QVariant();
}

QVariant IconTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Key");
        case 1:
            return tr("Icon");
        case 2:
            return tr("Map Symbol");
        }
    }
    return QVariant();
}

Qt::ItemFlags IconTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

IconTab::IconTab(Settings *settings, QWidget *parent) :
    QWidget(parent), mySettings(settings)
{
    IconTableModel *model = new IconTableModel(mySettings->mapIcons());
    QGridLayout *control = new QGridLayout();
    tab = new QTableView();
    tab->setModel(model);
    control->addWidget(tab, 0, 0);
    setLayout(control);
}

SettingsDialog::SettingsDialog(const Settings& settings, QWidget *parent) :
        QDialog(parent), mySettings(settings)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    tabWidget = new QTabWidget();
    gpsTab = new GpsTab(&mySettings);
    tabWidget->addTab(gpsTab, tr("&GPS Device"));
    trackTab = new DirTab(&mySettings);
    tabWidget->addTab(trackTab, tr("&Directories"));
    printTab = new PrintTab(&mySettings);
    tabWidget->addTab(printTab, tr("&Print"));
    iconTab = new IconTab(&mySettings);
    tabWidget->addTab(iconTab, tr("&Symbols"));
    mainLayout->addWidget(tabWidget);
    QGridLayout *control = new QGridLayout();
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}



void SettingsDialog::accept() {
    qDebug()<<"accept";
    mySettings.setGpsbabel(gpsTab->gpsbabel());
    mySettings.setGpsDevice(gpsTab->gpsDevice());
    mySettings.setGpsInterface(gpsTab->gpsInterface());
    mySettings.setTrackDir(trackTab->trackDir());
    mySettings.setSrtmDir(trackTab->srtmDir());
    mySettings.setTileSize(printTab->tileSize());
    mySettings.setOutTrackWidth(printTab->trackWidth());
    mySettings.setOutTrackColor(printTab->trackColor());
    QDialog::accept();
}

QPixmap SettingsDialog::colorIcon(const QColor& col) {
    QPixmap res(16, 16);
    res.fill(Qt::white);
    QPainter painter(&res);
    painter.fillRect(QRect(4, 4, 8, 8), col);
    return res;
}
