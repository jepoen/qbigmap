#include <QtWidgets>
#include "photodlg.h"

PhotoDlg::PhotoDlg(const QString& fileName, QWidget *parent) :
    QDialog(parent),
    myFileName(fileName), myPixmap(fileName)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QScrollArea *scrollArea = new QScrollArea();
    photoLabel = new QLabel();
    photoLabel->setPixmap(myPixmap);
    scrollArea->setWidget(photoLabel);
    QScrollBar *hsb = scrollArea->horizontalScrollBar();
    hsb->setValue(hsb->maximum()/2);
    QScrollBar *vsb = scrollArea->verticalScrollBar();
    vsb->setValue(vsb->maximum()/2);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
    resize(600, 600);
}
