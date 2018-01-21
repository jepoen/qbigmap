#include <QtWidgets>
#include <QtDebug>
#include "mapiconeditdlg.h"

MapIconEditDlg::MapIconEditDlg(QWidget *parent):
    QDialog(parent)
{
    QGridLayout *layout = new QGridLayout();
    myName = new QLineEdit;
    QLabel *lName = new QLabel(tr("&Name:"));
    lName->setBuddy(myName);
    layout->addWidget(lName, 0, 0);
    layout->addWidget(myName, 0, 1);

    cInternalMenuIcons = new QComboBox();
    bSelMenuFile = new QToolButton();
    bSelMenuFile->setIcon(QIcon(":/icons/disk.png"));
    gMenuIcon = new QButtonGroup();
    gMenuIcon->setExclusive(true);
    bMenuIntern = new QRadioButton(tr("Menu Icon internal"));
    gMenuIcon->addButton(bMenuIntern, SEL_INTERN);
    layout->addWidget(bMenuIntern, 1, 0);
    layout->addWidget(cInternalMenuIcons, 1, 1);
    bMenuFile = new QRadioButton(tr("Menu Icon file"));
    gMenuIcon->addButton(bMenuFile, SEL_FILE);
    lMenuPath = new QLabel();
    layout->addWidget(bMenuFile, 2, 0);
    layout->addWidget(lMenuPath);
    layout->addWidget(bSelMenuFile, 2, 2);
    lMenuIcon = new QLabel();
    layout->addWidget(lMenuIcon, 3, 1);

    cInternalMapIcons = new QComboBox();
    bSelMapFile = new QToolButton();
    bSelMapFile->setIcon(QIcon(":/icons/disk.png"));
    gMapIcon = new QButtonGroup();
    gMapIcon->setExclusive(true);
    bMapIntern = new QRadioButton(tr("Map Icon internal"));
    gMapIcon->addButton(bMapIntern, SEL_INTERN);
    layout->addWidget(bMapIntern, 4, 0);
    layout->addWidget(cInternalMapIcons, 4, 1);
    bMapFile = new QRadioButton(tr("Map Icon file"));
    gMapIcon->addButton(bMapFile, SEL_FILE);
    lMapPath = new QLabel();
    layout->addWidget(bMapFile, 5, 0);
    layout->addWidget(lMapPath, 5, 1);
    layout->addWidget(bSelMapFile, 5, 2);
    lMapIcon = new QLabel();
    layout->addWidget(lMapIcon, 6, 1);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);
    layout->addWidget(box, 10, 0, 1, 2);
    setLayout(layout);
    showInternalIcons();
    connect(cInternalMenuIcons, SIGNAL(currentIndexChanged(int)), this, SLOT(selInternalMenuIcon(int)));
    connect(bSelMenuFile, SIGNAL(clicked(bool)), this, SLOT(selInternalMenuFile()));
    connect(cInternalMapIcons, SIGNAL(currentIndexChanged(int)), this, SLOT(selInternalMapIcon(int)));
    connect(bSelMapFile, SIGNAL(clicked(bool)), this, SLOT(selInternalMapFile()));
    connect(box, SIGNAL(accepted()), this, SLOT(checkAccept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    connect(gMenuIcon, SIGNAL(buttonClicked(int)), this, SLOT(selMenuIcon(int)));
    selInternalMenuIcon(0);
    selInternalMapIcon(0);
    bMenuIntern->click();
    bMapIntern->click();
}

void MapIconEditDlg::setIcon(const MapIcon &icon) {
    myMenuIconPath = icon.icoFile();

    myName->setText(icon.name());
    int idx = cInternalMapIcons->findData(myMenuIconPath);
    qDebug()<<"setIcon"<<myMenuIconPath<<idx;
    if (idx >= 0) {
        cInternalMenuIcons->setCurrentIndex(idx);
        bMenuIntern->click();
    } else {
        bMenuFile->click();
    }
    myMapIconPath = icon.mapIcoFile();
    idx = cInternalMenuIcons->findData(myMapIconPath);
    if (idx >= 0) {
        cInternalMapIcons->setCurrentIndex(idx);
        bMapIntern->click();
    } else {
        bMapFile->click();
    }

}

void MapIconEditDlg::showInternalIcons() {
    QDir iconDir(":/mapicons");
    foreach (const QString& name, iconDir.entryList()) {
        QString icoPath = iconDir.filePath(name);
        QPixmap ico(icoPath);
        cInternalMenuIcons->addItem(ico, QString("(%1x%2)").arg(ico.width()).arg(ico.height()), icoPath);
        cInternalMapIcons->addItem(ico, QString("(%1x%2)").arg(ico.width()).arg(ico.height()), icoPath);
    }
}

void MapIconEditDlg::selInternalMenuIcon(int idx) {
    myMenuIconPath= cInternalMenuIcons->itemData(idx).toString();
    lMenuIcon->setPixmap(QPixmap(myMenuIconPath));
}

void MapIconEditDlg::selInternalMenuFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon File"), QString(), tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty()) {
        myMenuIconPath = fileName;
        lMenuPath->setText(myMenuIconPath);
        lMenuIcon->setPixmap(myMenuIconPath);
    }

}

void MapIconEditDlg::selMenuIcon(int idx) {
    if (idx == SEL_INTERN) {
        cInternalMenuIcons->setEnabled(true);
        int pos = cInternalMenuIcons->currentIndex();
        myMenuIconPath = cInternalMenuIcons->itemData(pos).toString();
    } else {
        cInternalMenuIcons->setEnabled(false);
        myMenuIconPath = lMenuPath->text();
    }
    lMenuIcon->setPixmap(myMenuIconPath);
}

void MapIconEditDlg::selInternalMapIcon(int idx) {
    myMapIconPath= cInternalMapIcons->itemData(idx).toString();
    lMapIcon->setPixmap(QPixmap(myMapIconPath));
}

void MapIconEditDlg::selInternalMapFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon File"), QString(), tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty()) {
        myMapIconPath = fileName;
        lMapPath->setText(myMapIconPath);
        lMapIcon->setPixmap(myMapIconPath);
    }

}

void MapIconEditDlg::selMapIcon(int idx) {
    if (idx == SEL_INTERN) {
        cInternalMapIcons->setEnabled(true);
        int pos = cInternalMapIcons->currentIndex();
        myMapIconPath = cInternalMapIcons->itemData(pos).toString();
    } else {
        cInternalMapIcons->setEnabled(false);
        myMapIconPath = lMapPath->text();
    }
    lMapIcon->setPixmap(myMapIconPath);
}

void MapIconEditDlg::checkAccept() {
    if (myName->text().trimmed().isEmpty()) return;
    if (myMenuIconPath.isEmpty()) return;
    emit accept();
}

MapIcon MapIconEditDlg::getIcon() const {
    QString name = myName->text().trimmed();
    if (name.isEmpty() || myMenuIconPath.isEmpty()) return MapIcon();
    return MapIcon(name, myMenuIconPath, myMapIconPath, myMapIconPath);
}
