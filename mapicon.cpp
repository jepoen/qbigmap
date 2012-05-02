#include "mapicon.h"

MapIcon::MapIcon(const QString &name, const QString &icoFile, const QString &mapFile) :
    myName(name), myIcoFile(icoFile), myMapIcoFile(mapFile)
{
    myIco = QPixmap(myIcoFile);
    myMapIco = QPixmap(myMapIcoFile);
}

void MapIcon::setMapIcoFile(const QString &fileName) {
    myMapIcoFile = fileName;
    myMapIco = QPixmap(myMapIcoFile);
}

const MapIcon& MapIconList::icon(const QString &key) const {
    foreach (const MapIcon& ico, myIcons) {
        if (ico.name() == key) return ico;
    }
    return myIcons[0];
}

void MapIconList::setMapIco(int idx, const QString &fileName) {
    myIcons[idx].setMapIcoFile(fileName);
}
