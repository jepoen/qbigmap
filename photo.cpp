#include <cassert>
#include <cstdio>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <QtGui>
#include <QtDebug>
#include <QLocale>
#include "photo.h"

ExifGps::ExifGps(const QPointF &coord): myCoord(coord) {
    if (myCoord.x() >= 0.0) myLonRef = "E";
    else {
        myLonRef = "W";
        myCoord.setX(-myCoord.x());
    }
    if (myCoord.y() >= 0.0) myLatRef = "N";
    else {
        myLatRef = "S";
        myCoord.setY(-myCoord.y());
    }
    QLocale locale("C");
    int degLon = int(myCoord.x());
    int minLon = int((myCoord.x()-degLon)*60);
    double secLon = ((myCoord.x()-degLon)*60-minLon)*60;
    myLonStr = QString("%1/1 %2/1 %3/100").arg(degLon).arg(minLon).arg(int(secLon*100));
    int degLat = int(myCoord.y());
    int minLat = int((myCoord.y()-degLat)*60);
    double secLat = ((myCoord.y()-degLat)*60-minLat)*60;
    myLatStr = QString("%1/1 %2/1 %3/100").arg(degLat).arg(minLat).arg(int(secLat*100));
    qDebug()<<myCoord<<" "<<myLonRef<<myLonStr<<" "<<myLatRef<<myLatStr;
}

ExifGps::ExifGps(const QString &lonRef, const QString &lon, const QString &latRef, const QString &lat):
    myLonRef(lonRef), myLonStr(lon),
    myLatRef(latRef), myLatStr(lat)
{
    double deg, min, sec;
    int decD, decM, decS;
    sscanf(lon.toStdString().c_str(), "%lf/%d %lf/%d %lf/%d", &deg, &decD, &min, &decM, &sec, &decS);
    if (decD > 0 && decM > 0 && decS > 0) myCoord.setX(deg/decD+min/decM/60+sec/decS/3600);
    sscanf(lat.toStdString().c_str(), "%lf/%d %lf/%d %lf/%d", &deg, &decD, &min, &decM, &sec, &decS);
    if (decD > 0 && decM > 0 && decS > 0) myCoord.setY(deg/decD+min/decM/60+sec/decS/3600);
}

Photo::Photo(const QString &fileName) :
        myFileName(fileName),
        myHasCoord(false)
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
        myTimestamp.setTimeSpec(Qt::UTC);
    }
    qDebug()<<myTimestamp;
    it = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitudeRef"));
    if (it == end) return;
    QString lonRef(it->value().toString().c_str());
    it = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude"));
    if (it == end) return;
    QString lon(it->value().toString().c_str());
    it = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef"));
    if (it == end) return;
    QString latRef(it->value().toString().c_str());
    it = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"));
    if (it == end) return;
    QString lat(it->value().toString().c_str());
    ExifGps gps(lonRef, lon, latRef, lat);
    myHasCoord = true;
    myCoord = gps.coord();
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
        QPixmap px(myFileName);
        if (px.height() > px.width())
            myPixmap = px.scaledToHeight(200);
        else
            myPixmap = px.scaledToWidth(200);
    }
    return myPixmap;
}

void Photo::fixTimestamp(int offset) {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(myFileName.toStdString().c_str());
    assert(image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    Exiv2::ExifData::const_iterator it = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    Exiv2::ExifData::const_iterator end = exifData.end();
    if (it == end) return;
    myTimestamp = myTimestamp.addSecs(offset);
    exifData["Exif.Photo.DateTimeOriginal"] = myTimestamp.toString("yyyy:MM:dd HH:mm:ss").toStdString();
    image->setExifData(exifData);
    image->writeMetadata();
}

void Photo::setGeoPos(const QPointF &pos) {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(myFileName.toStdString().c_str());
    assert(image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    ExifGps gps(pos);
    exifData["Exif.GPSInfo.GPSLatitudeRef"] = gps.latRef().toStdString();
    exifData["Exif.GPSInfo.GPSLatitude"] = gps.latStr().toStdString();
    exifData["Exif.GPSInfo.GPSLongitudeRef"] = gps.lonRef().toStdString();
    exifData["Exif.GPSInfo.GPSLongitude"] = gps.lonStr().toStdString();
    image->setExifData(exifData);
    image->writeMetadata();
    myHasCoord = true;
    myCoord = gps.coord();
}

bool operator<(const Photo& a, const Photo& b) {
    return a.timestamp() < b.timestamp();
}

bool operator==(const Photo& a, const Photo& b) {
    return a.timestamp() == b.timestamp();
}
