#include "mapicon.h"

MapIcon::MapIcon(const QString &name, const QString &icoFile, const QString &mapDefaultIco, const QString &mapFile) :
    myName(name), myIcoFile(icoFile), myMapDefaultIco(mapDefaultIco), myMapIcoFile(mapFile)
{
    myIco = QPixmap(myIcoFile);
    myMapIco = QPixmap(myMapIcoFile);
}

void MapIcon::setMapIcoFile(const QString &fileName) {
    myMapIcoFile = fileName;
    myMapIco = QPixmap(myMapIcoFile);
}

int MapIconList::iconIdx(const QString &key) const {
    for (int i = 0; i < myIcons.size(); i++) {
        if (myIcons[i].name() == key) return i;
    }
    return -1;
}

const MapIcon& MapIconList::icon(const QString &key) const {
    int idx = iconIdx(key);
    if (idx >= 0) return myIcons[idx];
    return myIcons[0];
}

void MapIconList::setMapIco(int idx, const QString &fileName) {
    myIcons[idx].setMapIcoFile(fileName);
}
