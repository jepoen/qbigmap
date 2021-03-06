#include <QtWidgets>
#include <QtDebug>
#include "settingsdialog.h"
#include "mapiconeditdlg.h"

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
    eUseSrtm = new QCheckBox(tr("&Use SRTM data"));
    eUseSrtm->setChecked(mySettings->useSrtm());
    control->addWidget(eUseSrtm, 1, 0, 1, 2);
    QLabel *lSrtmDir = new QLabel(tr("&SRTM data directory:"));
    eSrtmDir = new QLineEdit(mySettings->srtmDir());
    eSrtmDir->setEnabled(mySettings->useSrtm());
    lSrtmDir->setBuddy(eSrtmDir);
    bSrtmDir = new QPushButton(tr("Select"));
    control->addWidget(lSrtmDir, 2, 0);
    control->addWidget(eSrtmDir, 2, 1);
    control->addWidget(bSrtmDir, 2, 2);
    QLabel *lExportDir = new QLabel(tr("GPX e&xport directory:"));
    lExportDir->setToolTip(tr("Directory for simplified tracks and online maps"));
    eExportDir = new QLineEdit(mySettings->exportDir());
    lExportDir->setBuddy(eExportDir);
    bExportDir = new QPushButton(tr("Select"));
    control->addWidget(lExportDir, 3, 0);
    control->addWidget(eExportDir, 3, 1);
    control->addWidget(bExportDir, 3, 2);
    QLabel *lCacheDir = new QLabel(tr("Tile Cache directory:"));
    lCacheDir->setToolTip(tr("Directory for caching downloaded map tiles"));
    eCacheDir = new QLineEdit(mySettings->cacheDir());
    lCacheDir->setBuddy(eCacheDir);
    bCacheDir = new QPushButton(tr("Select"));
    control->addWidget(lCacheDir, 4, 0);
    control->addWidget(eCacheDir, 4, 1);
    control->addWidget(bCacheDir, 4, 2);
    setLayout(control);
    connect(eUseSrtm, SIGNAL(clicked(bool)), eSrtmDir, SLOT(setEnabled(bool)));
    connect(bTrackDir, SIGNAL(clicked()), this, SLOT(selectTrackDir()));
    connect(bSrtmDir, SIGNAL(clicked()), this, SLOT(selectSrtmDir()));
    connect(bExportDir, SIGNAL(clicked()), this, SLOT(selectExportDir()));
    connect(bCacheDir, SIGNAL(clicked()), this, SLOT(selectCacheDir()));
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

void DirTab::selectExportDir() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select SRTM dir"), mySettings->exportDir());
    if (!dirName.isEmpty()) {
        eExportDir->setText(dirName);
    }
}

void DirTab::selectCacheDir() {
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Cache dir"), mySettings->cacheDir());
    if (!dirName.isEmpty()) {
        eCacheDir->setText(dirName);
    }
}

const QString DirTab::trackDir() const {
    return eTrackDir->text();
}

bool DirTab::useSrtm() const {
    return eUseSrtm->isChecked();
}

const QString DirTab::srtmDir() const {
    return eSrtmDir->text();
}

const QString DirTab::exportDir() const {
    return eExportDir->text();
}

const QString DirTab::cacheDir() const {
    return eCacheDir->text();
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
    QLabel *lPrintBorder = new QLabel(tr("&Print border (mm):"));
    bPrintBorder = new QSpinBox();
    bPrintBorder->setRange(0, 50);
    bPrintBorder->setValue(mySettings->printBorder());
    lPrintBorder->setBuddy(bPrintBorder);
    control->addWidget(lPrintBorder, 1, 0);
    control->addWidget(bPrintBorder, 1, 1);
    QLabel *lTrackWidth = new QLabel(tr("Track &width (pt):"));
    control->addWidget(lTrackWidth, 2, 0);
    bTrackWidth = new QSpinBox();
    bTrackWidth->setRange(0, 20);
    bTrackWidth->setValue(mySettings->outTrackWidth());
    lTrackWidth->setBuddy(bTrackWidth);
    control->addWidget(bTrackWidth, 2, 1);
    QLabel *lTrackColor = new QLabel(tr("Track &color:"));
    control->addWidget(lTrackColor, 3, 0);
    myTrackColor = mySettings->outTrackColor();
    bTrackColor = new QPushButton(SettingsDialog::colorIcon(myTrackColor), tr("Select"));
    lTrackColor->setBuddy(bTrackColor);
    control->addWidget(bTrackColor, 3, 1);
    QLabel *lRouteWidth = new QLabel(tr("&Route width (pt):"));
    control->addWidget(lRouteWidth, 4, 0);
    bRouteWidth = new QSpinBox();
    bRouteWidth->setRange(0, 20);
    bRouteWidth->setValue(mySettings->outRouteWidth());
    lRouteWidth->setBuddy(bRouteWidth);
    control->addWidget(bRouteWidth, 4, 1);
    QLabel *lRouteColor = new QLabel(tr("Route c&olor:"));
    control->addWidget(lRouteColor, 5, 0);
    myRouteColor = mySettings->outRouteColor();
    bRouteColor = new QPushButton(SettingsDialog::colorIcon(myRouteColor), tr("Select"));
    lRouteColor->setBuddy(bRouteColor);
    control->addWidget(bRouteColor, 5, 1);
    setLayout(control);
    connect(bTrackColor, SIGNAL(clicked()), this, SLOT(selTrackColor()));
    connect(bRouteColor, SIGNAL(clicked()), this, SLOT(selRouteColor()));
}

int PrintTab::tileSize() const {
    return bTileSize->value();
}

int PrintTab::printBorder() const {
    return bPrintBorder->value();
}

int PrintTab::trackWidth() const {
    return bTrackWidth->value();
}

QColor PrintTab::trackColor() const {
    return myTrackColor;
}

int PrintTab::routeWidth() const {
    return bRouteWidth->value();
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

SelectFileDelegate::SelectFileDelegate(QObject *parent) :
    QItemDelegate(parent)
{}

QWidget *SelectFileDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    qDebug()<<"createEditor "<<index;
    if (index.column() == 2) {
        QFileDialog *dlg = new QFileDialog();
        dlg->exec();
        //return dlg;
    }
    return QItemDelegate::createEditor(parent, option, index);
}

void SelectFileDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    qDebug()<<"setEditorData "<<index;
    if (index.column() == 2) {
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void SelectFileDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (index.column() == 2) {

    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void SelectFileDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QItemDelegate::updateEditorGeometry(editor, option, index);
}

IconTableModel::IconTableModel(const MapIconList &icons, QObject *parent) :
    QAbstractTableModel(parent), myIcons(icons)
{}

IconTableModel::~IconTableModel() {
    qDebug()<<"~IconTableModel";
}

int IconTableModel::rowCount(const QModelIndex &/*parent*/) const {
    return myIcons.size();
}

int IconTableModel::columnCount(const QModelIndex &/*parent*/) const {
    return 4;
}

bool IconTableModel::insertRow(int row, const QModelIndex &parent) {
    beginInsertRows(parent, row, row);
    myIcons.insert(row, MapIcon());
    endInsertRows();
    return true;
}

bool IconTableModel::removeRow(int row, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row);
    myIcons.remove(row);
    endInsertRows();
    return true;
}

QVariant IconTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    int idx = index.row();
    const MapIcon& ico = myIcons.icons().at(idx);
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0:
            return ico.name();
        case 1:
            //return ico.mapIcoFile();
            return QString();
        }
    } else if (role == Qt::DecorationRole) {
        switch(index.column()) {
        case 0:
            return ico.ico();
        case 1:
            return ico.mapIco();
        case 2:
            return QIcon(":/icons/disk.png");
        case 3:
            return QIcon(":/icons/delete.png");
        }
    } else if (role == Qt::ToolTipRole) {
        switch(index.column()) {
        case 1:
            return ico.mapIcoFile();
        case 2:
            return tr("Edit Icon");
        case 3:
            return tr("Delete Icon");
        default:
            return "";
        }
    }
    return QVariant();
}

bool IconTableModel::setData(const QModelIndex &index, const QVariant& /*value*/, int role) {
    if (role == Qt::EditRole) {
        /*
        if (value == QVariant()) { // reset
            qDebug()<<"setData reset "<<myIcons.icons().at(index.row()).icoFile();
            myIcons.setMapIco(index.row(), myIcons.icons().at(index.row()).mapDefaultIco());
        } else {
            myIcons.setMapIco(index.row(), value.toString());
        }
        */
        QModelIndex newIndex(createIndex(index.row(), 1));
        emit dataChanged(newIndex, newIndex);
        return true;
    }
    return false;
}

QVariant IconTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Key");
        case 1:
            return tr("Map Symbol");
        case 2:
            return tr("Edit");
        case 3:
            return tr("Delete");
        }
    }
    return QVariant();
}

Qt::ItemFlags IconTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index);
}

void IconTab::add() {
    MapIconEditDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        MapIcon ico = dlg.getIcon();
        int len = myModel.iconListSize();
        myModel.insertRow(len);
        myModel.setIcon(len, ico);
        QModelIndex index = myModel.index(len, 0, QModelIndex());
        myModel.setData(index, ico.icoFile(), Qt::EditRole);
        //layerTab->setCurrentIndex(index);
    }
}

void IconTab::edit(const QModelIndex &index) {
    if (index.column() == 2) {
        MapIconEditDlg dlg(this);
        int idx = index.row();
        const MapIcon& ico = myModel.iconAt(idx);
        qDebug()<<"Icon:"<<ico.name();
        dlg.setIcon(ico);
        if (dlg.exec() == QDialog::Accepted) {
            MapIcon ico = dlg.getIcon();
            myModel.setIcon(idx, ico);
        }
    } else if (index.column() == 3) {
        MapIcon ico = myModel.iconAt(index.row());
        if (QMessageBox::question(this, tr("Delete Icon"), tr("Delete icon %1").arg(ico.name()), QMessageBox::Yes|QMessageBox::No, QMessageBox::NoButton)
                == QMessageBox::Yes) {
            myModel.removeRow(index.row());
            if (index.row() >= mySettings->mapIconList().size()) {
                QModelIndex idx = myModel.index(myModel.iconListSize()-1, 0, QModelIndex());
                myModel.setData(idx, QVariant(), Qt::EditRole);
            }
        }
        //myModel.setData(index, QVariant(), Qt::EditRole);
    }
}

IconTab::IconTab(Settings *settings, QWidget *parent) :
    QWidget(parent), mySettings(settings), myModel(mySettings->mapIconList())
{
    QGridLayout *control = new QGridLayout();
    myNewIco = new QPushButton(QIcon(":/icons/add.png"), tr("Add"));
    connect(myNewIco, SIGNAL(pressed()), this, SLOT(add()));
    tab = new QTableView();
    tab->setModel(&myModel);
    tab->verticalHeader()->hide();
    tab->horizontalHeader()->setStretchLastSection(true);
    connect(tab, SIGNAL(clicked(QModelIndex)), this, SLOT(edit(QModelIndex)));
    resizeTab();
    connect(&myModel, SIGNAL(layoutChanged()), this, SLOT(resizeTab()));
    control->addWidget(myNewIco, 0, 0);
    control->addWidget(tab, 1, 0);
    setLayout(control);
}

void IconTab::resizeTab() {
    tab->resizeColumnToContents(0);
    tab->resizeColumnToContents(1);
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
    mySettings.setUseSrtm(trackTab->useSrtm());
    mySettings.setSrtmDir(trackTab->srtmDir());
    mySettings.setExportDir(trackTab->exportDir());
    mySettings.setCacheDir(trackTab->cacheDir());
    mySettings.setTileSize(printTab->tileSize());
    mySettings.setPrintBorder(printTab->printBorder());
    mySettings.setOutTrackWidth(printTab->trackWidth());
    mySettings.setOutTrackColor(printTab->trackColor());
    mySettings.setOutRouteWidth(printTab->routeWidth());
    mySettings.setOutRouteColor(printTab->routeColor());
    mySettings.setMapIconList(iconTab->mapIconList());
    QDialog::accept();
}

QPixmap SettingsDialog::colorIcon(const QColor& col) {
    QPixmap res(16, 16);
    res.fill(Qt::white);
    QPainter painter(&res);
    painter.fillRect(QRect(4, 4, 8, 8), col);
    return res;
}
