// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <time.h>

#include "scan.h"
#include "globaldeclarations.h"

Scan::Scan(HttpGet* parent) 
  : HttpGet(parent)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

Scan::~Scan() 
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void Scan::get(QString uuid, QString tip) 
{
    QString url = LOGIN_SERVER_HOST + WX_CGI_PATH + "login?uuid=" + uuid + 
        "&tip=" + tip + "&_=" + QString::number(time(NULL));
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

void Scan::finished(QNetworkReply* reply) 
{
    QString replyStr(reply->readAll());
    QString qrredirect_uriStr = "window.redirect_uri=\"";
    QString redirect_uriStr = "";
    int index = -1;

#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif
    if (replyStr == "window.code=408;") {
        Q_EMIT error("timeout");
        return;
    }
    if (replyStr == "window.code=201;") {
        Q_EMIT scanedButWaitConfirm();
        return;
    }
    if (replyStr.contains("window.code=200;")) {
        index = replyStr.indexOf(qrredirect_uriStr) + qrredirect_uriStr.size();
        if (index == -1) {
            qWarning() << "ERROR:" << __PRETTY_FUNCTION__ << 
                "redirect_uri not found!";
            Q_EMIT scanedAndConfirmed(redirect_uriStr);
            return;
        }
        redirect_uriStr = replyStr.mid(index, replyStr.size() - index - 2);
#if QWX_DEBUG
        qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << redirect_uriStr;
#endif
        Q_EMIT scanedAndConfirmed(redirect_uriStr);
        return; 
    }
}
