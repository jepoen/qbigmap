#include "gpxprofiledlg.h"
#include "settings.h"
#include <QtGui>
#include <QtDebug>

GpxProfileDlg::GpxProfileDlg(const GpxPointList &gpxPoints, Settings *settings, QWidget *parent) :
    QDialog(parent),
    myGpxPoints(gpxPoints),
    myProfile(gpxPoints, settings),
    myBorder(60), myScaleX(10), myScaleY(0.3)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    eProfile = new QLabel();
    QScrollArea *scroller = new QScrollArea();
    scroller->setWidget(eProfile);
    mainLayout->addWidget(scroller);
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
    eTextWidth->setValue(150);
    eTextWidth->setSingleStep(10);
    lTextWidth->setBuddy(eTextWidth);
    control->addWidget(lTextWidth, 2, 0);
    control->addWidget(eTextWidth, 2, 1);
    QLabel *lTop = new QLabel(tr("Top &offset:"));
    eTop = new QSpinBox();
    eTop->setRange(0, 200);
    eTop->setValue(60);
    eTop->setSingleStep(1);
    lTop->setBuddy(eTop);
    control->addWidget(lTop, 2, 2);
    control->addWidget(eTop, 2, 3);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    connect(eWidth, SIGNAL(valueChanged(int)), this, SLOT(computePixmap()));
    connect(eHeight, SIGNAL(valueChanged(int)), this, SLOT(computePixmap()));
    connect(eText, SIGNAL(toggled(bool)), this, SLOT(fixWidth(bool)));
    connect(eTextWidth, SIGNAL(valueChanged(int)), this, SLOT(computePixmap()));
    connect(eTop, SIGNAL(valueChanged(int)), this, SLOT(computePixmap()));
    computePreferredSize();
    computePixmap();
}

void GpxProfileDlg::computePreferredSize() {
    BoundingBox bb = Gpx::boundingBox(myGpxPoints);
    int width = myBorder+int(bb.len()*myScaleX);
    int height = myBorder+int((bb.ele().y()-bb.ele().x())*myScaleY);
    qDebug()<<"ele "<<bb.ele()<<" dist "<<bb.len()<<" w "<<width<<" h "<<height;
    if (Gpx::hasSym(myGpxPoints)) {
        eText->setChecked(true);
        width += eTextWidth->value();
        eTextWidth->setEnabled(true);
        eTop->setEnabled(true);
    } else {
        eText->setChecked(false);
        eText->setEnabled(false);
        eTextWidth->setEnabled(false);
        eTop->setEnabled(false);
    }
    eWidth->setValue(width);
    eHeight->setValue(height);
}

void GpxProfileDlg::fixWidth(bool val) {
    int w = eWidth->value();
    if (val) w += eTextWidth->value();
    else     w -= eTextWidth->value();
    eWidth->setValue(w);
    computePixmap();
}

void GpxProfileDlg::computePixmap() {
    QPixmap pixmap(eWidth->value(), eHeight->value());
    myProfile.paint(&pixmap, eWidth->value(), eHeight->value(), topOffset(), textWidth());
    eProfile->setPixmap(pixmap);
    eProfile->resize(pixmap.width(), pixmap.height());
}

QSize GpxProfileDlg::pixmapSize() const {
    return QSize(eWidth->value(), eHeight->value());
}

int GpxProfileDlg::textWidth() const {
    if (eText->isChecked()) return eTextWidth->value();
    else return 0;
}

int GpxProfileDlg::topOffset() const {
    if (eText->isChecked()) return eTop->value();
    else return 0;
}

const QPixmap* GpxProfileDlg::pixmap() const {
    return eProfile->pixmap();
}
