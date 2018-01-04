#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QtDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("plastique");
    QTranslator appTranslator;
    qDebug()<<QLocale::system().name();
    appTranslator.load("qbigmap_"+QLocale::system().name(), ":/translations");
    a.installTranslator(&appTranslator);
    QTranslator qtTranslator;
    qtTranslator.load("qt_"+QLocale::system().name(), ":/translations");
    a.installTranslator(&qtTranslator);
    MainWindow w;
    w.show();
    return a.exec();
}
