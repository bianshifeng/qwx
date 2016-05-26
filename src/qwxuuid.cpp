// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <time.h>

#include "qwxuuid.h"
#include "globaldeclarations.h"

QwxUUID::QwxUUID(HttpGet* parent)
  : HttpGet(parent)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    get();
}

QwxUUID::~QwxUUID()
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void QwxUUID::get()
{
    QString url = LOGIN_SERVER_HOST + "/jslogin?appid=wx782c26e4c19acffb"
        "&redirect_uri=" + WX_SERVER_HOST + WX_CGI_PATH + "webwxnewloginpage"
        "&fun=new&lang=zh_CN&_=" + QString::number(time(NULL));
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url);
}

void QwxUUID::finished(QNetworkReply* reply)
{
    QString replyStr(reply->readAll());
    QString uuidStr = "";
    QString qruuidStr = "window.QRLogin.uuid = \"";
    int index = -1;

#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif
    // TODO: window.QRLogin.code = 200; window.QRLogin.uuid = "395bb96e535e47"; 
    index = replyStr.indexOf(qruuidStr) + qruuidStr.size();
    if (index == -1) {
        qWarning() << "ERROR:" << __PRETTY_FUNCTION__ << "uuid not found!";
        Q_EMIT error();
        return;
    }
    uuidStr = replyStr.mid(index, replyStr.size() - index - QString("\";").size());
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << uuidStr;
#endif
    if (uuidStr == "") {
        Q_EMIT error();
        return;
    }
    Q_EMIT uuidChanged(uuidStr);
}
