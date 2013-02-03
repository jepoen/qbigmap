#include <cassert>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <QtGui>
#include <QtDebug>
#include "photo.h"

Photo::Photo(const QString &fileName) :
        myFileName(fileName)
{
    readTimestamp();
}

void Photo::readTimestamp() {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(myFileName.toStdString().c_str());
    assert(image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    Exiv2::ExifData::const_iterator it = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    Exiv2::ExifData::const_iterator end = exifData.end();
    if (it != end) {
        qDebug()<<QString(it->value().toString().c_str());
        myTimestamp = QDateTime::fromString(QString(it->value().toString().c_str()), "yyyy:MM:dd HH:mm:ss");
    }
    qDebug()<<myTimestamp;
//    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
//        const char *tn = i->typeName();
//        std::cerr<<tn<<": "<<i->key()<<": "<<i->tag()<<": "<<i->value()<<std::endl;
//    }
}

QString Photo::baseFileName() const {
    return QFileInfo(myFileName).fileName();
}

const QPixmap& Photo::pixmap() {
    if (myPixmap.isNull()) {
        myPixmap = QPixmap(myFileName).scaledToWidth(200);
    }
    return myPixmap;
}

bool operator<(const Photo& a, const Photo& b) {
    return a.timestamp() < b.timestamp();
}

bool operator==(const Photo& a, const Photo& b) {
    return a.timestamp() == b.timestamp();
}
