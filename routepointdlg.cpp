#include <QtGui>
#include "routepointdlg.h"

RoutePointDlg::RoutePointDlg(const RoutePoint &point, const QList<MapIcon> &icons, QWidget *parent) :
    QDialog(parent),
    myPoint(point)
{
    setWindowTitle(tr("Edit Route Point"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lSym = new QLabel(tr("&Symbol:"));
    eSym = new QComboBox();
    createSymList(eSym, icons);
    lSym->setBuddy(eSym);
    control->addWidget(lSym, 0, 0);
    control->addWidget(eSym, 0, 1);
    QLabel *lName = new QLabel(tr("&Name:"));
    eName = new QLineEdit();
    eName->setText(myPoint.name());
    lName->setBuddy(eName);
    control->addWidget(lName, 1, 0);
    control->addWidget(eName, 1, 1);
    QLabel *lDesc = new QLabel(tr("&Description:"));
    eDesc = new QLineEdit();
    eDesc->setText(myPoint.desc());
    lDesc->setBuddy(eDesc);
    control->addWidget(lDesc, 2, 0);
    control->addWidget(eDesc, 2, 1);
    QLabel *lLink = new QLabel(tr("&Link:"));
    eLink = new QLineEdit();
    eLink->setText(myPoint.link());
    lLink->setBuddy(eLink);
    control->addWidget(lLink, 3, 0);
    control->addWidget(eLink, 3, 1);
    eErr = new QLabel();
    control->addWidget(eErr, 4, 0, 1, 2, Qt::AlignCenter);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(check()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

void RoutePointDlg::createSymList(QComboBox *box, const QList<MapIcon>& icons) {
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

void RoutePointDlg::check() {
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

RoutePoint RoutePointDlg::point() const {
    int symIdx = eSym->currentIndex();
    QString sym = eSym->itemText(symIdx);
    return RoutePoint(myPoint.coord(), sym, eName->text(), eDesc->text(), eLink->text());
}
