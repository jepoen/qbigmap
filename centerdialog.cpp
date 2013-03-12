#include <QtGui>
#include <QtDebug>
#include "centerdialog.h"

CenterItem::CenterItem(QGraphicsItem *parent) :
        QGraphicsItem(parent)
{

}

QRectF CenterItem::boundingRect() const {
    return QRectF(-20, -20, 40, 40);
}

void CenterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen(Qt::cyan);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawLine(-20, 0, 20, 0);
    painter->drawLine(0, -20, 0, 20);
    painter->drawEllipse(-15, -15, 30, 30);
}

CenterDialog::CenterDialog(const QPointF &lonLat, MapScene *scene, QWidget *parent) :
    QDialog(parent), myLonLat(lonLat), mapScene(scene), mySave(false)
{
    setWindowTitle(tr("Set map center"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *controls = new QGridLayout();
    QLabel *lLon = new QLabel(tr("Longitude"));
    controls->addWidget(lLon, 0, 0);
    bLon = new QDoubleSpinBox();
    bLon->setRange(-180, 180);
    bLon->setDecimals(5);
    bLon->setSingleStep(0.001);
    bLon->setValue(lonLat.x());
    controls->addWidget(bLon, 0, 1);
    QLabel *lLat = new QLabel(tr("Latitude"));
    controls->addWidget(lLat, 1, 0);
    bLat = new QDoubleSpinBox();
    bLat->setRange(-85, 85);
    bLat->setDecimals(5);
    bLat->setSingleStep(0.001);
    bLat->setValue(lonLat.y());
    controls->addWidget(bLat, 1, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Ok);
    QPushButton *saveButton = buttonBox->addButton(tr("Save as default"), QDialogButtonBox::ApplyRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(bLon, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
    connect(bLat, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAndAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this, SIGNAL(finished(int)), this, SLOT(hide()));
    mainLayout->addLayout(controls);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    Model *model = mapScene->model();
    QPoint pos = model->lonLat2Scene(lonLat);
    centerItem = new CenterItem();
    centerItem->setPos(pos);
    centerItem->setZValue(1);
    scene->addItem(centerItem);

}

void CenterDialog::hide() {
    qDebug()<<"hide center dialog";
    mapScene->removeItem(centerItem);
    delete centerItem;
}

void CenterDialog::changeValue(double val) {
    Q_UNUSED(val);
    myLonLat.setX(bLon->value());
    myLonLat.setY(bLat->value());
    qDebug()<<"new pos"<<myLonLat;
    Model *model = mapScene->model();
    QPoint pos = model->lonLat2Scene(myLonLat);
    centerItem->setPos(pos);
}

void CenterDialog::saveAndAccept() {
    mySave = true;
    accept();
}
