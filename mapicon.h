#ifndef MAPICON_H
#define MAPICON_H

#include <QString>
#include <QPixmap>

class MapIcon
{
private:
    QString myName;
    QString myIcoFile;
    QString myMapIcoFile;
    QPixmap myIco;
    QPixmap myMapIco;
public:
    MapIcon(const QString& name, const QString& icoFile, const QString& mapFile);
    const QString& name() const { return myName; }
    const QPixmap& ico() const { return myIco; }
    const QString& icoFile() const { return myIcoFile; }
    const QString& mapIcoFile() const { return myMapIcoFile; }
};

#endif // MAPICON_H
