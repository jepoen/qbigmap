#ifndef SRTM_H
#define SRTM_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QString>

class QProgressDialog;

class SrtmEntry : public QObject
{
    Q_OBJECT
private:
    int myLon;
    int myLat;
    int *myData;
    QNetworkAccessManager netManager;
    QNetworkReply *myReply;
    QString mySrtmDir;
    QFile *myFile;
    QProgressDialog *progressDlg;

    void readFile(const QString& srtmDir);
    QString subdir() const;
    void download();
    void unzip(const QString& fileName);

public:
    static const QString baseUrl;
    SrtmEntry(int lon, int lat, const QString& srtmDir):
        myLon(lon), myLat(lat), myData(0), myFile(0),
        progressDlg(0)
    {
        readFile(srtmDir);
    }
    ~SrtmEntry() { delete [] myData; }
    QString getFileName(const QString& srtmEntry) const;
    int lon0() const { return myLon; }
    int lat0() const { return myLat; }
    /**
      * Get elevation relative to lon0, lat0.
      */
    int offEle(double offLon, double offLat) const;
    int ele(double lon, double lat) const { return offEle(lon-myLon, lat-myLat); }
signals:
    void finished();
private slots:
    void httpReadyRead();
    void httpFinished();
    void updateProgress(qint64 bytesRead, qint64 bytesTotal);
};

#endif // SRTM_H
