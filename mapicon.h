#ifndef MAPICON_H
#define MAPICON_H

#include <QString>
#include <QPixmap>

class MapIcon
{
private:
    QString myName;
    QString myIcoFile;
    QString myMapDefaultIco;
    QString myMapIcoFile;
    QPixmap myIco;
    QPixmap myMapIco;
public:
    MapIcon(const QString& name="", const QString& icoFile="", const QString& mapDefaultIco="", const QString& mapFile="");
    const QString& name() const { return myName; }
    const QPixmap& ico() const { return myIco; }
    const QString& icoFile() const { return myIcoFile; }
    void setIcoFile(const QString& fileName) { myIcoFile = fileName; }
    const QString& mapIcoFile() const { return myMapIcoFile; }
    void setMapIcoFile(const QString& fileName);
    const QString& mapDefaultIco() const { return myMapDefaultIco; }
    const QPixmap& mapIco() const { return myMapIco; }
    bool isNull() const { return myName == ""; }
};

class MapIconList {
private:
    QList<MapIcon> myIcons;
public:
    MapIconList(const QList<MapIcon>& icons = QList<MapIcon>()) : myIcons(icons)
    {}
    const MapIcon &icon(const QString& key) const;
    const QList<MapIcon>& icons() const { return myIcons; }
    void setIcons(const QList<MapIcon>& icons) { myIcons = icons; }
    void setMapIco(int idx, const QString& fileName);
    int size() const { return myIcons.size(); }
};

#endif // MAPICON_H
