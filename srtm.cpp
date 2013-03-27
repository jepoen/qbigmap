#include <QtGui>
#include <QtDebug>
#include <QtNetwork>
#include "srtm.h"

const QString SrtmEntry::baseUrl = "http://www.viewfinderpanoramas.org/dem3/";

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
    return QString("%1/%2%3%4%5.hgt").arg(srtmDir).arg(cLat).arg(lat, 2, 10, QChar('0'))
            .arg(cLon).arg(lon, 3, 10, QChar('0'));
}

void SrtmEntry::readFile(const QString &srtmDir) {
    mySrtmDir = srtmDir;
    QFile srtmFile(getFileName(srtmDir));
    if (!srtmFile.exists()) download();
    qDebug()<<"SRTM File "<<srtmFile.fileName();
    if (!srtmFile.open(QIODevice::ReadOnly)) return;
    QByteArray data = srtmFile.readAll();
    qDebug()<<"data size "<<data.size();
    srtmFile.close();
    myData = new int[1201*1201];
    for (int i = 0; i < 1201*1201; i++) {
        int v = static_cast<unsigned char>(data[2*i])*256 + static_cast<unsigned char>(data[2*i+1]);
        if (v > 32767) v -= 65536;
        if (v == -32768) {
            qDebug()<<"missing value at "<<i%1201<<i/1201;
        }
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

QString SrtmEntry::subdir() const {
    int x = (myLon+180)/6+1;
    QString prefix = "";
    int lat = myLat;
    if (myLat < 0) {
        lat = -lat;
        prefix = "S";
    }
    lat /= 4;
    return QString("%1%2%3").arg(prefix).arg(char('A'+lat)).arg(x, 2, 10, QChar('0'));
}

void SrtmEntry::download() {
    QString url = QString("%1%2.zip").arg(baseUrl).arg(subdir());
    qDebug()<<url;
    progressDlg = new QProgressDialog();
    progressDlg->setLabelText(tr("Download %1").arg(url));
    myReply = netManager.get(QNetworkRequest(QUrl(url)));
    QString fileName = QString("%1/%2.zip").arg(mySrtmDir).arg(subdir());
    myFile = new QFile(fileName);
    myFile->open(QIODevice::WriteOnly);
    QEventLoop pause;
    connect(myReply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(myReply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(myReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgress(qint64, qint64)));
    connect(this, SIGNAL(finished()), &pause, SLOT(quit()));
    pause.exec();
    qDebug()<<"download complete";
}

void SrtmEntry::httpReadyRead() {
    myFile->write(myReply->readAll());
}

void SrtmEntry::httpFinished() {
    myFile->flush();
    myFile->close();
    QString fileName = myFile->fileName();
    delete myFile;
    myFile = 0;
    myReply->deleteLater();
    myReply = 0;
    progressDlg->hide();
    progressDlg->deleteLater();
    progressDlg = 0;
    unzip(fileName);
    emit finished();
}

void SrtmEntry::updateProgress(qint64 bytesRead, qint64 bytesTotal) {
    progressDlg->setMaximum(bytesTotal);
    progressDlg->setValue(bytesRead);
}

void SrtmEntry::unzip(const QString &fileName) {
    QProcess pUnzip;

    pUnzip.start("unzip", QStringList()<<"-j"<<"-o"<<"-d"<<mySrtmDir<<fileName);
    if (!pUnzip.waitForStarted()) {
        qDebug()<<"Unzip start failed "<<fileName;
        return;
    }
    if (!pUnzip.waitForFinished()) {
        qDebug()<<"Unzip process failed "<<fileName;
        return;
    }
}
