#include <QtGui>
#include "gpxpointdlg.h"

GpxPointDlg::GpxPointDlg(const GpxPoint &point, const QList<MapIcon> &icons, QWidget *parent) :
    QDialog(parent),
    myPoint(point)
{
    setWindowTitle(tr("Edit Route Point"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    //control->setColumnMinimumWidth(1, 300);
    QLabel *lEle = new QLabel(tr("&Elevation (m):"));
    eEle = new QLineEdit();
    eEle->setValidator(new QIntValidator());
    eEle->setText(QString("%1").arg(point.ele()));
    lEle->setBuddy(eEle);
    QLabel *lSrtmEle = new QLabel(tr("SRTM elevation:"));
    eSrtmEle = new QLabel();
    control->addWidget(lEle, 0, 0);
    control->addWidget(eEle, 0, 1);
    control->addWidget(lSrtmEle, 0, 2);
    control->addWidget(eSrtmEle, 0, 3);
    QLabel *lSym = new QLabel(tr("&Symbol:"));
    eSym = new QComboBox();
    createSymList(eSym, icons);
    lSym->setBuddy(eSym);
    control->addWidget(lSym, 1, 0);
    control->addWidget(eSym, 1, 1);
    QLabel *lName = new QLabel(tr("&Name:"));
    eName = new QLineEdit();
    eName->setMinimumSize(100, 0);
    eName->setText(myPoint.name());
    lName->setBuddy(eName);
    control->addWidget(lName, 2, 0);
    control->addWidget(eName, 2, 1, 1, 3);
    QLabel *lDesc = new QLabel(tr("&Description:"));
    eDesc = new QLineEdit();
    eDesc->setText(myPoint.desc());
    lDesc->setBuddy(eDesc);
    control->addWidget(lDesc, 3, 0);
    control->addWidget(eDesc, 3, 1, 1, 3);
    QLabel *lLink = new QLabel(tr("&Link:"));
    eLink = new QLineEdit();
    eLink->setText(myPoint.link());
    lLink->setBuddy(eLink);
    control->addWidget(lLink, 4, 0);
    control->addWidget(eLink, 4, 1, 1, 3);
    if (myPoint.type() != GpxPoint::WPT) {
        QLabel *lDist0 = new QLabel("Distance from start:");
        eDist0 = new QLabel();
        control->addWidget(lDist0, 5, 0);
        control->addWidget(eDist0, 5, 1);
        QLabel *lDist1 = new QLabel("Distance to end:");
        eDist1 = new QLabel();
        control->addWidget(lDist1, 5, 2);
        control->addWidget(eDist1, 5, 3);
    }
    eErr = new QLabel();
    control->addWidget(eErr, 7, 0, 1, 4, Qt::AlignCenter);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(check()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

void GpxPointDlg::createSymList(QComboBox *box, const QList<MapIcon>& icons) {
    int idx = 0;
    box->addItem("");
    foreach(const MapIcon& ico, icons) {
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

void GpxPointDlg::check() {
    if (!eEle->hasAcceptableInput()) {
        eErr->setText("Elevation should be an int value!");
        return;
    }
    int symIdx = eSym->currentIndex();
    QString name = eName->text().trimmed();
    QString desc = eDesc->text().trimmed();
    QString link = eLink->text().trimmed();
    if (symIdx > 0 && !name.isEmpty()) {
        emit accept();
        return;
    }
    if (symIdx == 0 && name.isEmpty() && desc.isEmpty() && link.isEmpty()) {
        emit accept();
        return;
    }
    eErr->setText(tr("Enter nothing or at least Name and Symbol!"));
}

GpxPoint GpxPointDlg::point() const {
    int symIdx = eSym->currentIndex();
    QString sym = eSym->itemText(symIdx);
    return GpxPoint(myPoint.type(), myPoint.coord(), myPoint.timestamp(), eEle->text().toDouble(), sym, eName->text(), eDesc->text(), eLink->text());
}

