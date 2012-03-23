#ifndef PHOTO_H
#define PHOTO_H
#include <QDateTime>
#include <QIcon>
#include <QPixmap>
#include <QString>

class Photo
{
private:
    QString myFileName;
    QPixmap myPixmap;
    QIcon myIcon;
    QDateTime myTimestamp;
    void readTimestamp();
public:
    Photo(const QString& fileName);
    const QString& filename() const { return myFileName; }
    const QIcon& icon() const { return myIcon; }
    const QPixmap& pixmap() const { return myPixmap; }
    const QDateTime& timestamp() const { return myTimestamp; }
};

bool operator<(const Photo& a, const Photo& b);
bool operator==(const Photo& a, const Photo& b);
#endif // PHOTO_H
