#ifndef HTTPGET_H
#define HTTPGET_H

#include <QObject>
#include <QPoint>
#include <QString>
#include <QBuffer>
#include <QHttp>
#include <QUrl>

class TileRequest {
private:
    QString myKey;
    QPoint myPos;
    int myZ;
public:
    TileRequest() {}
    TileRequest(const QString& key, const QPoint& pos, int z);
    const QString& key() { return myKey; }
    const QPoint& pos() { return myPos; }
    const int& z() { return myZ; }
    bool operator==(TileRequest req) { return this->key() == req.key(); }
};

class HttpGet : public QObject
{
    Q_OBJECT
private:
    QUrl myUrl;
    int myReqId;
    QHttp *myHttp;
    QBuffer *myBuffer;
    TileRequest myRequest;
    int myStatusCode;
    bool reqAbort;
public:
    HttpGet(QObject *parent = 0);
    virtual ~HttpGet();
    bool getFile(const QUrl& url, const TileRequest& request);
    const QByteArray& getData() const { return myBuffer->data(); }
    void close() { delete myBuffer; myBuffer = NULL; }
    const TileRequest& request() { return myRequest; }
    int statusCode() { return myStatusCode; }
signals:
    void done(bool error);
    void finish();
private slots:
    void finished(int, bool);
    void changeState(int);
    void getState(QHttpResponseHeader header);
public slots:
    void abort();
};

#endif // HTTPGET_H
