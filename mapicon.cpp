#include "mapicon.h"

MapIcon::MapIcon(const QString &name, const QString &icoFile, const QString &mapFile) :
    myName(name), myIcoFile(icoFile), myMapIcoFile(mapFile)
{
    myIco = QPixmap(myIcoFile);
    myMapIco = QPixmap(myMapIcoFile);
}
