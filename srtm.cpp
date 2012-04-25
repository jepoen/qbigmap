#include <QtCore>
#include <QtDebug>
#include "srtm.h"

QString SrtmEntry::getFileName(const QString &srtmDir) const {
    int lon = myLon;
    QString cLon("E");
    int lat = myLat;
    QString cLat("N");
    if (lon < 0) {
        cLon = "W";
        lon = -lon;
    }
    if (lat < 0) {
        cLat = "S";
        lat = -lat;
    }
    return QString("%1/%2%3%4%5.cor").arg(srtmDir).arg(cLat).arg(lat, 2, 10, QChar('0'))
            .arg(cLon).arg(lon, 3, 10, QChar('0'));
}

void SrtmEntry::readFile(const QString &srtmDir) {
    QFile srtmFile(getFileName(srtmDir));
    qDebug()<<"SRTM File "<<srtmFile.fileName();
    if (!srtmFile.open(QIODevice::ReadOnly)) return;
    QByteArray data = srtmFile.readAll();
    qDebug()<<"data size "<<data.size();
    srtmFile.close();
    myData = new int[1201*1201];
    for (int i = 0; i < 1201*1201; i++) {
        int v = static_cast<unsigned char>(data[2*i])*256 + static_cast<unsigned char>(data[2*i+1]);
        if (v > 32767) v -= 65536;
        myData[i] = v;
    }
    qDebug()<<"readFile complete";
}

int SrtmEntry::offEle(double offLon, double offLat) const {
    if (myData == 0) return 0;
    if (offLon < 0 || offLon >= 1.0 || offLat < 0 || offLat >= 1.0) return 0;
    int iLon0 = int(offLon*1200);
    int iLon1 = iLon0+1;
    double dLon = offLon*1200.0-iLon0;
    int iLat0 = int(offLat*1200);
    double dLat = offLat*1200-iLat0;
    iLat0 = 1199-iLat0;
    int iLat1 = iLat0-1;
    qDebug()<<"offEle iLon0 "<<iLon0<<" iLat0 "<<iLat0;
    double v0 = (1.0-dLon)*myData[iLat0*1201+iLon0]+dLon*myData[iLat0*1201+iLon1];
    double v1 = (1.0-dLon)*myData[iLat1*1201+iLon0]+dLon*myData[iLat1*1201+iLon1];
    return int(round((1.0-dLat)*v0+dLat*v1));
}
