#include <QtWidgets>
#include <QPrinter>
#include "settings.h"
#include "mapprintdlg.h"

MapPrintDlg::MapPrintDlg(QPrinter *printer, Settings *settings, QSize imgSize, QWidget *parent) :
    QDialog(parent),
    myDevice(printer), mySettings(settings), myImgSize(imgSize)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    QFormLayout *control = new QFormLayout();
    eTileSize = new QSpinBox();
    eTileSize->setRange(5, 200);
    eTileSize->setValue(mySettings->tileSize());
    control->addRow(tr("&Tilesize (mm):"), eTileSize);
    ePrintBorder = new QSpinBox();
    ePrintBorder->setRange(0, 20);
    ePrintBorder->setValue(mySettings->printBorder());
    control->addRow(tr("&Print Border (mm):"), ePrintBorder);
    oPages = new QLabel();
    control->addRow(tr("Pages:"), oPages);
    oTiles = new QLabel();
    control->addRow(tr("Tiles per Page:"), oTiles);
    oMargins = new QLabel();
    control->addRow(tr("Map margins (mm):"), oMargins);
    mainLayout->addLayout(control);
    QDialogButtonBox *box = new QDialogButtonBox();
    QPushButton *bSave = box->addButton(tr("&Save settings"), QDialogButtonBox::ApplyRole);
    connect(bSave, SIGNAL(clicked()), this, SLOT(saveSettings()));
    box->addButton(QDialogButtonBox::Ok);
    box->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    computeLayout();
    connect(eTileSize, SIGNAL(valueChanged(int)), this, SLOT(computeLayout()));
    connect(ePrintBorder, SIGNAL(valueChanged(int)), this, SLOT(computeLayout()));
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

void MapPrintDlg::computeLayout() {
    double tilesize = eTileSize->value(); // mm
    int dpix = myDevice->logicalDpiX();
    int dpiy = myDevice->logicalDpiY();
    qDebug()<<"width: "<<myDevice->width()<<" "<<dpix;
    double deviceW = myDevice->width()*25.4/dpix;
    double deviceH = myDevice->height()*25.4/dpiy;
    double bd = ePrintBorder->value();
    int maxw = int((deviceW-2*bd)/tilesize);
    int maxh = int((deviceH-2*bd)/tilesize);
    myTilesPerPage = QSize(maxw, maxh);
    int pagesx = (myImgSize.width()/256-1)/maxw+1;
    int pagesy = (myImgSize.height()/256-1)/maxh+1;
    myPages = QSize(pagesx, pagesy);
    double margW = 0.5*(deviceW-maxw*tilesize);
    double margH = 0.5*(deviceH-maxh*tilesize);
    myDevBorder = QPointF(bd*dpix/25.4, bd*dpiy/25.4);
    myDevMargins = QPointF(margW*dpix/25.4, margH*dpiy/25.4);
    oPages->setText(trUtf8("%1 × %2").arg(pagesx).arg(pagesy));
    oTiles->setText(trUtf8("%1 × %2").arg(maxw).arg(maxh));
    oMargins->setText(tr("%1 / %2").arg(int(margW)).arg(int(margH)));
}

void MapPrintDlg::saveSettings() {
    mySettings->setPrintBorder(ePrintBorder->value());
    mySettings->setTileSize(eTileSize->value());
    QMainWindow *win = qobject_cast<QMainWindow *>(parent());
    if (win != 0) mySettings->save(win);
}
