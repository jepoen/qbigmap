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
    int iconIdx(const QString& key) const;
    const QList<MapIcon>& icons() const { return myIcons; }
    void setIcons(const QList<MapIcon>& icons) { myIcons = icons; }
    void setMapIco(int idx, const QString& fileName);
    int size() const { return myIcons.size(); }
    void append(const MapIcon& ico) { myIcons.append(ico); }
    MapIcon at(int idx) const { return myIcons.at(idx); }
    void insert(int idx, const MapIcon& ico) { myIcons.insert(idx, ico); }
    void setIco(int idx, const MapIcon& ico) { myIcons[idx] = ico; }
    void remove(int idx) { myIcons.removeAt(idx); }
};

#endif // MAPICON_H
