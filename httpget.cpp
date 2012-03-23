#include <iostream>
#include <QtCore>
#include <QtDebug>
#include "httpget.h"

TileRequest::TileRequest(const QString &key, const QPoint &pos, int z) :
        myKey(key), myPos(pos), myZ(z)
{}

HttpGet::HttpGet(QObject *parent) :
        QObject(parent)
{
    myHttp = new QHttp();
    myBuffer = NULL;
    connect(myHttp, SIGNAL(requestFinished(int,bool)), this, SLOT(finished(int, bool)));
    connect(myHttp, SIGNAL(stateChanged(int)), this, SLOT(changeState(int)));
    connect(myHttp, SIGNAL(responseHeaderReceived(QHttpResponseHeader)), this, SLOT(getState(QHttpResponseHeader)));
}

HttpGet::~HttpGet() {
    qDebug()<<"destructor "<<myRequest.key();
    delete myBuffer;
    myHttp->abort();
    delete myHttp;
    qDebug()<<"complete";
}

bool HttpGet::getFile(const QUrl &url, const TileRequest& request) {
    qDebug()<<"getFile ";
    if (myBuffer != NULL)
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
    myHttp->setHost(myUrl.host(), myUrl.port(80));
    myBuffer->open(QBuffer::ReadWrite);
    reqAbort = false;
    myReqId = myHttp->get(myUrl.path(), myBuffer);
    //myHttp->close();
    return true;
}

void HttpGet::finished(int reqId, bool error) {
    if (reqId != myReqId)
        return;
    qDebug()<<"done: "<<myRequest.key()<<" size: "<<myBuffer->data().size();
    qDebug()<<"cancel: "<<reqAbort;
    if (reqAbort || myStatusCode != 200)
        error = false;
    if (error) {
        std::cerr<<qPrintable(myHttp->errorString())<<std::endl;
    }
    emit done(error);
}

void HttpGet::changeState(int state) {
    qDebug()<<"state: "<<state<<" "<<myRequest.key()<<" size: "<<myBuffer->data().size();
    if (state == QHttp::Unconnected) {
        emit finish();
        //myBuffer.close();
    }
}

void HttpGet::getState(QHttpResponseHeader header) {
    myStatusCode = header.statusCode();
}

void HttpGet::abort() {
    qDebug()<<"abort";
    myHttp->abort();
    reqAbort = true;
}
