#include "helpwindow.h"
#include <QtGui>
#include <QtDebug>

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Help"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    myText = new QTextBrowser(this);
    mainLayout->addWidget(myText);
    setLayout(mainLayout);
}

HelpWindow::~HelpWindow() {
    qDebug()<<"helpWindow destroyed";
}

void HelpWindow::setUrl(const QString &url) {
    myUrl = url;
    myText->setSource(QUrl::fromLocalFile(myUrl));
}
