#include <QtWidgets>
#include <QtDebug>
#include "helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent)
{
    //setStyleSheet("QToolButton {border: 1px solid;};");
    setWindowTitle(tr("Help"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *control = new QHBoxLayout();
    bHome = createToolButton(":/icons/house.png", tr("Go to main page"));
    control->addWidget(bHome);
    bBack = createToolButton(":/icons/arrow_left.png", tr("Go back"));
    control->addWidget(bBack);
    bForward = createToolButton(":/icons/arrow_right.png", tr("Go forward"));
    control->addWidget(bForward);
    control->addStretch();
    bClose = createToolButton(":/icons/delete.png", "Close Help Window");
    control->addWidget(bClose);
    mainLayout->addLayout(control);
    myText = new QTextBrowser(this);
    mainLayout->addWidget(myText);
    setLayout(mainLayout);
    connect(bHome, SIGNAL(clicked()), myText, SLOT(home()));
    connect(bBack, SIGNAL(clicked()), myText, SLOT(backward()));
    connect(bForward, SIGNAL(clicked()), myText, SLOT(forward()));
    connect(bClose, SIGNAL(clicked()), this, SLOT(close()));
    resize(600, 600);
}

QToolButton *HelpWindow::createToolButton(const QString &ico, const QString &toolTip) {
    QToolButton *b = new QToolButton();
    b->setIcon(QIcon(ico));
    b->setToolTip(toolTip);
    b->setAutoRaise(true);
    return b;
}

HelpWindow::~HelpWindow() {
    qDebug()<<"helpWindow destroyed";
}

void HelpWindow::setUrl(const QString &url) {
    myUrl = url;
    myText->setSource(QUrl::fromLocalFile(myUrl));
}
