// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include <QJsonDocument>                                                           
#include <QJsonObject>

#include "xiaodoubi.h"
#include "globaldeclarations.h"

XiaoDouBi::XiaoDouBi(HttpGet* parent) 
  : HttpGet(parent)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

XiaoDouBi::~XiaoDouBi() 
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
}

void XiaoDouBi::get(QString word) 
{ 
    QString url = XIAODOUBI_URL + word;
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    HttpGet::get(url); 
}

void XiaoDouBi::finished(QNetworkReply* reply) 
{
    Q_EMIT contentChanged(QString(reply->readAll()));
}
