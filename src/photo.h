#ifndef PHOTO_H
#define PHOTO_H

#include <QDateTime>
#include <QIcon>
#include <QPixmap>
#include <QPointF>
#include <QString>

class ExifGps {
private:
    QPointF myCoord;
    QString myLonRef;
    QString myLonStr;
    QString myLatRef;
    QString myLatStr;
public:
    explicit ExifGps(const QPointF& coord);
    explicit ExifGps(const QString& lonRef, const QString& lon, const QString& latRef, const QString& lat);
    QPointF coord() const { return myCoord; }
    QString lonRef() const { return myLonRef; }
    QString lonStr() const { return myLonStr; }
    QString latRef() const { return myLatRef; }
    QString latStr() const { return myLatStr; }
};

class Photo
{
private:
    QString myFileName;
    QPixmap myPixmap;
    QIcon myIcon;
    QDateTime myTimestamp;
    bool myHasCoord;
    QPointF myCoord;
    void readTimestamp();
public:
    Photo(const QString& fileName);
    const QString& filename() const { return myFileName; }
    QString baseFileName() const;
    const QIcon& icon() const { return myIcon; }
    const QPixmap& pixmap();
    const QDateTime& timestamp() const { return myTimestamp; }
    bool hasCoord() const { return myHasCoord; }
    QPointF coord() const { return myCoord; }
    void fixTimestamp(int offset);
    void setGeoPos(const QPointF& pos);
};

bool operator<(const Photo& a, const Photo& b);
bool operator==(const Photo& a, const Photo& b);
#endif // PHOTO_H
