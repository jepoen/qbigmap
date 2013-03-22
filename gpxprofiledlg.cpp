#include "gpxprofiledlg.h"
#include <QtGui>
#include <QtDebug>

GpxProfileDlg::GpxProfileDlg(const GpxPointList &gpxPoints, QWidget *parent) :
    QDialog(parent),
    myGpxPoints(gpxPoints),
    myScaleX(10), myScaleY(0.5)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *control = new QGridLayout();
    QLabel *lWidth = new QLabel(tr("&Width:"));
    eWidth = new QSpinBox();
    eWidth->setRange(100, 5000);
    eWidth->setSingleStep(10);
    lWidth->setBuddy(eWidth);
    control->addWidget(lWidth, 0, 0);
    control->addWidget(eWidth, 0, 1);
    QLabel *lHeight = new QLabel(tr("&Height:"));
    eHeight = new QSpinBox();
    eHeight->setRange(100, 5000);
    eHeight->setSingleStep(10);
    lHeight->setBuddy(eHeight);
    control->addWidget(lHeight, 0, 2);
    control->addWidget(eHeight, 0, 3);
    mainLayout->addLayout(control);
    eText = new QCheckBox(tr("&Show Symbols and text"));
    control->addWidget(eText, 1, 0, 1, 2);
    QLabel *lTextWidth = new QLabel(tr("&Text width:"));
    eTextWidth = new QSpinBox();
    eTextWidth->setRange(50, 500);
    eTextWidth->setValue(100);
    eTextWidth->setSingleStep(10);
    lTextWidth->setBuddy(eTextWidth);
    control->addWidget(lTextWidth, 1, 2);
    control->addWidget(eTextWidth, 1, 3);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    computePreferredSize();
}

void GpxProfileDlg::computePreferredSize() {
    BoundingBox bb = Gpx::boundingBox(myGpxPoints);
    int width = int(bb.len()*myScaleX);
    int height = int((bb.ele().y()-bb.ele().x())*myScaleY);
    qDebug()<<"ele "<<bb.ele()<<" dist "<<bb.len()<<" w "<<width<<" h "<<height;
    if (Gpx::hasSym(myGpxPoints)) {
        eText->setChecked(true);
        width += eTextWidth->value();
    } else {
        eText->setChecked(false);
        eText->setEnabled(false);
        eTextWidth->setEnabled(false);
    }
    eWidth->setValue(width);
    eHeight->setValue(height);
}

QSize GpxProfileDlg::pixmapSize() const {
    return QSize(eWidth->value(), eHeight->value());
}

int GpxProfileDlg::textWidth() const {
    if (eText->isChecked()) return eTextWidth->value();
    else return 0;
}
