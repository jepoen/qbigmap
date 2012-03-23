#include <libexif/exif-data.h>
#include <QtGui>
#include <QtDebug>
#include "photo.h"

Photo::Photo(const QString &fileName) :
        myFileName(fileName)
{
    readTimestamp();
    QPixmap px(myFileName);
    int w = px.width();
    int h = px.height();
    if (w < h && h > 180) {
        myIcon = QIcon(px.scaledToHeight(180));
    //    myPixmap = px.scaledToHeight(180);
    } else if (w > 200) {
        myIcon = QIcon(px.scaledToWidth(200));
    //    myPixmap = px.scaledToWidth(200);
    } else {
        myIcon = QIcon(px);
    //    myPixmap = px;
    }
}

void Photo::readTimestamp() {
    char buf[1024];
    buf[0] = '\0';
    ExifData *ed = exif_data_new_from_file(myFileName.toStdString().c_str());
    if (!ed) return;
    ExifEntry *entry = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME);
    exif_entry_get_value(entry, buf, sizeof(buf));
    QString sTimestamp(buf);
    myTimestamp = QDateTime::fromString(sTimestamp, QString("yyyy:MM:dd HH:mm:ss"));
    qDebug()<<myFileName<<": "<<myTimestamp<<" ("<<sTimestamp<<")";
}

bool operator<(const Photo& a, const Photo& b) {
    return a.timestamp() < b.timestamp();
}

bool operator==(const Photo& a, const Photo& b) {
    return a.timestamp() == b.timestamp();
}
