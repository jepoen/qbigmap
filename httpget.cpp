#include <iostream>
#include <QtGui>
#include <QtDebug>
#include <QtNetwork>
#include "httpget.h"

TileRequest::TileRequest(const QString &key, const QPoint &pos, int z) :
        myKey(key), myPos(pos), myZ(z)
{}

HttpGet::HttpGet(QObject *parent) :
        QObject(parent),
        reply(0),
        myBuffer(0)
{}

HttpGet::~HttpGet() {
    qDebug()<<"destructor "<<myRequest.key();
    delete myBuffer;
    reply->deleteLater();
    qDebug()<<"complete";
}

bool HttpGet::getFile(const QUrl &url, const TileRequest &request, QProgressDialog *dlg) {
    qDebug()<<"getFile ";
    delete myBuffer;
    myBuffer = new QBuffer();
    myUrl = url;
    myRequest = request;
    if (!myUrl.isValid()) {
        std::cerr<<"Error: Invalid URL"<<std::endl;
        return false;
    }
    if (myUrl.path().isEmpty()) {
        std::cerr<<"Error: Path is empty"<<std::endl;
        return false;
    }
    myBuffer->open(QBuffer::ReadWrite);
    reply = netManager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(changeState(qint64, qint64)));
    connect(dlg, SIGNAL(canceled()), this, SLOT(abort()));
    reqAbort = false;
    return true;
}

void HttpGet::finished() {
    bool error;
    if (reqAbort) {
        if (myBuffer) {
            myBuffer->close();
            myBuffer->deleteLater();
            myBuffer = 0;
        }
        error = true;
        reply->deleteLater();
        reply = 0;
    } else {
        myBuffer->close();
        error = false;
        reply->deleteLater();
        reply = 0;
    }
    emit done(error);
}

void HttpGet::readData() {
    myBuffer->write(reply->readAll());
}

void HttpGet::changeState(qint64 bytesRead, qint64 bytesTotal) {
    qDebug()<<bytesRead<<" from "<<bytesTotal;
}

void HttpGet::abort() {
    qDebug()<<"abort";
    reply->abort();
    reqAbort = true;
}
