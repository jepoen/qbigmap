#include <QtWidgets>
#include "gpxpointdlg.h"
#include "gpxlinkdlg.h"
#include "model.h"

GpxPointDlg::GpxPointDlg(Model *model, const GpxPoint &point, const MapIconList &icons, bool enableCoordEdit, QWidget *parent) :
    QDialog(parent),
    myModel(model),
    myPoint(point)
{
    setWindowTitle(tr("Edit %1").arg(myPoint.typeName()));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    //control->setColumnMinimumWidth(1, 300);
    QLabel *lLon = new QLabel(tr("L&ongitude:"));
    control->addWidget(lLon, 0, 0);
    eLon = new QDoubleSpinBox();
    eLon->setRange(-180, 180);
    eLon->setDecimals(6);
    eLon->setSingleStep(0.0001);
    eLon->setValue(myPoint.coord().x());
    lLon->setBuddy(eLon);
    control->addWidget(eLon, 0, 1);
    QLabel *lLat = new QLabel(tr("L&atitude:"));
    control->addWidget(lLat, 0, 2);
    eLat = new QDoubleSpinBox();
    eLat->setRange(-90, 90);
    eLat->setDecimals(6);
    eLat->setSingleStep(0.0001);
    eLat->setValue(myPoint.coord().y());
    lLat->setBuddy(eLat);
    control->addWidget(eLat, 0, 3);
    QLabel *lEle = new QLabel(tr("&Elevation (m):"));
    eEle = new QLineEdit();
    eEle->setValidator(new QIntValidator());
    eEle->setText(QString("%1").arg(int(round(point.ele()))));
    lEle->setBuddy(eEle);
    QLabel *lSrtmEle = new QLabel(tr("SRTM elevation:"));
    eSrtmEle = new QLabel();
    control->addWidget(lEle, 2, 0);
    control->addWidget(eEle, 2, 1);
    control->addWidget(lSrtmEle, 2, 2);
    control->addWidget(eSrtmEle, 2, 3);
    QLabel *lSym = new QLabel(tr("&Symbol:"));
    eSym = new QComboBox();
    createSymList(eSym, icons);
    lSym->setBuddy(eSym);
    control->addWidget(lSym, 3, 0);
    control->addWidget(eSym, 3, 1);
    eShowMap = new QCheckBox(tr("Show in &Map"));
    eShowMap->setChecked(myPoint.showMap());
    control->addWidget(eShowMap, 3, 2);
    eShowProfile = new QCheckBox(tr("Show in &Profile"));
    eShowProfile->setChecked(myPoint.showProfile());
    control->addWidget(eShowProfile, 3, 3);
    QLabel *lName = new QLabel(tr("&Name:"));
    eName = new QLineEdit();
    eName->setMinimumSize(100, 0);
    eName->setText(myPoint.name());
    lName->setBuddy(eName);
    control->addWidget(lName, 4, 0);
    control->addWidget(eName, 4, 1, 1, 3);
    QLabel *lCmt = new QLabel(tr("&Comment:"));
    eCmt = new QPlainTextEdit();
    eCmt->setPlainText(myPoint.cmt());
    setTextHeight(eCmt, 6);
    lCmt->setBuddy(eCmt);
    control->addWidget(lCmt, 5, 0);
    control->addWidget(eCmt, 5, 1, 1, 3);
    QLabel *lDesc = new QLabel(tr("&Description:"));
    eDesc = new QPlainTextEdit();
    eDesc->setPlainText(myPoint.desc());
    setTextHeight(eDesc, 6);
    lDesc->setBuddy(eDesc);
    control->addWidget(lDesc, 6, 0);
    control->addWidget(eDesc, 6, 1, 1, 3);
    QLabel *lLink = new QLabel(tr("&Link:"));
    eLink = new QListWidget();
    foreach (const GpxLink& link, myPoint.links()) {
        eLink->addItem(QString("[%1] %2 (%3)").arg(link.url()).arg(link.text()).arg(link.mimeType()));
    }
    lLink->setBuddy(eLink);
    QVBoxLayout *linkControlLayout = new QVBoxLayout();
    QToolButton *bAddLink = new QToolButton();
    bAddLink->setIcon(QIcon(":/icons/add.png"));
    linkControlLayout->addWidget(bAddLink);
    connect(bAddLink, SIGNAL(clicked(bool)), this, SLOT(addLink()));
    QToolButton *bEditLink = new QToolButton();
    bEditLink->setIcon(QIcon(":/icons/pencil.png"));
    linkControlLayout->addWidget(bEditLink);
    connect(bEditLink, SIGNAL(clicked(bool)), this, SLOT(editLink()));
    QToolButton *bDelLink = new QToolButton();
    bDelLink->setIcon(QIcon(":/icons/delete.png"));
    linkControlLayout->addWidget(bDelLink);
    connect(bDelLink, SIGNAL(clicked(bool)), this, SLOT(delLink()));
    QToolButton *bUpLink = new QToolButton();
    bUpLink->setIcon(QIcon(":/icons/arrow_up.png"));
    linkControlLayout->addWidget(bUpLink);
    connect(bUpLink, SIGNAL(clicked(bool)), this, SLOT(upLink()));
    QToolButton *bDownLink = new QToolButton();
    bDownLink->setIcon(QIcon(":/icons/arrow_down.png"));
    linkControlLayout->addWidget(bDownLink);
    connect(bDownLink, SIGNAL(clicked(bool)), this, SLOT(downLink()));
    control->addWidget(lLink, 7, 0);
    control->addWidget(eLink, 7, 1, 1, 3);
    control->addLayout(linkControlLayout, 7, 4);
    QLabel *lGpxType = new QLabel(tr("&Type:"));
    eGpxType = new QLineEdit();
    lGpxType->setBuddy(eGpxType);
    eGpxType->setText(myPoint.gpxType());
    control->addWidget(lGpxType, 8, 0);
    control->addWidget(eGpxType, 8, 1, 1, 3);
    if (myPoint.type() != GpxPoint::WPT) {
        QLabel *lDist0 = new QLabel("Distance from start:");
        eDist0 = new QLabel();
        control->addWidget(lDist0, 9, 0);
        control->addWidget(eDist0, 9, 1);
        QLabel *lDist1 = new QLabel("Distance to end:");
        eDist1 = new QLabel();
        control->addWidget(lDist1, 9, 2);
        control->addWidget(eDist1, 9, 3);
    }
    eErr = new QLabel();
    control->addWidget(eErr, 10, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    if (!enableCoordEdit) {
        eLon->setEnabled(false);
        eLat->setEnabled(false);
        eEle->setEnabled(false);
    }
    connect(eLon, SIGNAL(valueChanged(double)), this, SLOT(changePos(double)));
    connect(eLat, SIGNAL(valueChanged(double)), this, SLOT(changePos(double)));
    connect(eSym, SIGNAL(currentIndexChanged(int)), this, SLOT(symChanged()));
    connect(box, SIGNAL(accepted()), this, SLOT(check()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    eSym->setFocus();
}

void GpxPointDlg::setTextHeight(QPlainTextEdit *view, int h) {
    QFontMetrics m(view->font()) ;
    int rowHeight = m.lineSpacing() ;
    view->setFixedHeight(h*rowHeight) ;
}

void GpxPointDlg::createSymList(QComboBox *box, const MapIconList& iconList) {
    int idx = 0;
    box->addItem("");
    foreach(const MapIcon& ico, iconList.icons()) {
        box->addItem(ico.ico(), ico.name());
        if (ico.name() == myPoint.sym()) {
            idx = box->count()-1;
        }
    }
    box->setCurrentIndex(idx);
}

void GpxPointDlg::setDists(double dist0, double dist1) {
    if (myPoint.type() == GpxPoint::WPT) return;
    eDist0->setText(QString("%1 km").arg(dist0, 0, 'f', 1));
    eDist1->setText(QString("%1 km").arg(dist1, 0, 'f', 1));
}

void GpxPointDlg::setSrtmEle(int ele) {
    eSrtmEle->setText(QString("%1 mNN").arg(ele));
}

void GpxPointDlg::changePos(double /*val*/) {
    QPointF pos(eLon->value(), eLat->value());
    myPoint.setSrtm(myModel->srtmEle(pos));
    setSrtmEle(myPoint.srtm());
    emit posChanged(pos);
}

void GpxPointDlg::addLink() {
    GpxLinkDlg dlg("", "", "", this);
    if (dlg.exec() == QDialog::Accepted) {
        GpxLink link(dlg.url(), dlg.mime(), dlg.text());
        myPoint.addLink(link);
        eLink->addItem(tr("[%1] %2 (%3)").arg(link.url()).arg(link.text()).arg(link.mimeType()));
    }
}

void GpxPointDlg::editLink() {
    int pos = eLink->currentIndex().row();
    if (pos < 0) return;
    const GpxLink link = myPoint.links().at(pos);
    GpxLinkDlg dlg(link.url(), link.text(), link.mimeType(), this);
    if (dlg.exec() == QDialog::Accepted) {
        GpxLink link(dlg.url(), dlg.mime(), dlg.text());
        myPoint.setLink(pos, link);
        eLink->takeItem(pos);
        eLink->insertItem(pos, tr("[%1] %2 (%3)").arg(link.url()).arg(link.text()).arg(link.mimeType()));
    }
}

void GpxPointDlg::delLink() {
    int pos = eLink->currentIndex().row();
    if (pos < 0) return;
    QString text = eLink->currentItem()->text();
    if (QMessageBox::warning(this, tr("Delete GPX Link"), tr("Delete link %1").arg(text),
                             QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        myPoint.delLink(pos);
        eLink->takeItem(pos);
    }
}

void GpxPointDlg::downLink() {
    int pos = eLink->currentIndex().row();
    if (pos < 0 || pos >= eLink->count()-1) return;
    myPoint.swapLinks(pos, pos+1);
    QListWidgetItem *item = eLink->takeItem(pos);
    eLink->insertItem(pos+1, item);
    eLink->setCurrentRow(pos+1);
}

void GpxPointDlg::upLink() {
    int pos = eLink->currentIndex().row();
    if (pos < 1) return;
    myPoint.swapLinks(pos-1, pos);
    QListWidgetItem *item = eLink->takeItem(pos-1);
    eLink->insertItem(pos, item);
    eLink->setCurrentRow(pos-1);
}

void GpxPointDlg::check() {
    if (!eEle->hasAcceptableInput()) {
        eErr->setText("Elevation should be an int value!");
        return;
    }
    int symIdx = eSym->currentIndex();
    QString name = eName->text().trimmed();
    QString cmt = eCmt->toPlainText().trimmed();
    QString desc = eDesc->toPlainText().trimmed();
    int linkCnt = eLink->count();
    //QString link = eLink->text().trimmed();
    if (symIdx > 0 && !name.isEmpty()) {
        emit accept();
        return;
    }
    if (symIdx == 0 && name.isEmpty() && desc.isEmpty() && linkCnt == 0 && cmt.isEmpty()) {
        emit accept();
        return;
    }
    eErr->setText(tr("Enter nothing or at least Name and Symbol!"));
}

GpxPoint GpxPointDlg::point() const {
    QPointF pos(eLon->value(), eLat->value());
    int symIdx = eSym->currentIndex();
    if (symIdx == 0) {
        eShowMap->setChecked(false);
        eShowProfile->setChecked(false);
    }
    QString sym = eSym->itemText(symIdx);
    GpxPoint p(myPoint.type(), pos, myPoint.timestamp(), eEle->text().toDouble(), sym,
               eName->text().trimmed(), eCmt->toPlainText().trimmed(), eDesc->toPlainText().trimmed(),
               myPoint.links(), eGpxType->text().trimmed());
    p.setSrtm(myPoint.srtm());
    p.setShowMap(eShowMap->isChecked());
    p.setShowProfile(eShowProfile->isChecked());
    return p;
}

void GpxPointDlg::symChanged() {
    int symIdx = eSym->currentIndex();
    if (symIdx == 0) {
        eShowMap->setChecked(false);
        eShowProfile->setChecked(false);
    } else {
        eShowMap->setChecked(true);
        eShowProfile->setChecked(true);
    }
}
