// Copyright (C) 2014 - 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#if QWX_DEBUG
#include <QFile>
#endif
#include <QJsonDocument>                                                           
#include <QJsonObject>                                                             
#include <QJsonArray>
#include <random>
#include <time.h>

#include "init.h"
#include "userobject.h"
#include "globaldeclarations.h"

Init::Init(HttpPost* parent) 
  : HttpPost(parent),
    m_v2(false)
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    std::mt19937 eng(time(NULL));
    std::uniform_int_distribution<long long> deviceId(1615250492, 519062714508114);
    m_deviceId = QString::number(deviceId(eng));
    Q_EMIT deviceIdChanged();
}

Init::~Init() 
{
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
#endif
    m_clear();
}

void Init::m_clear() 
{
    Q_FOREACH (QObject* obj, m_contactList) {
        if (obj) {
            delete obj;
            obj = Q_NULLPTR;
        }
    }
    m_contactList.clear();
    m_map.clear();
}

void Init::post(QString uin, QString sid, QString ticket) 
{
    m_clear();

    QString url = WX_SERVER_HOST + WX_CGI_PATH + "webwxinit?pass_ticket=" + 
        ticket + "&r=" + QString::number(time(NULL));
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    QString json = "{\"BaseRequest\":{\"Uin\":\"" + uin + "\",\"Sid\":\"" + 
        sid + "\",\"Skey\":\"\",\"DeviceID\":\"" + m_deviceId + "\"}}";
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << json;
#endif
    HttpPost::post(url, json, true);
}

void Init::postV2(QString uin, QString sid, QString ticket)
{
    m_v2 = true;

    m_clear();

    QString url = WX_V2_SERVER_HOST + WX_CGI_PATH + "webwxinit?pass_ticket=" +
        ticket + "&r=" + QString::number(time(NULL));
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << url;
#endif
    QString json = "{\"BaseRequest\":{\"Uin\":\"" + uin + "\",\"Sid\":\"" +
        sid + "\",\"Skey\":\"\",\"DeviceID\":\"" + m_deviceId + "\"}}";
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << json;
#endif
    HttpPost::post(url, json, true);
}

QString Init::loginHeadImgUrl() const 
{ 
    return "file://" + QWXDIR + "/" + m_loginUserName;
}

void Init::finished(QNetworkReply* reply) 
{
    QString replyStr = QString(reply->readAll());
#if QWX_DEBUG
    QFile file("init.json"); 
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << replyStr;
        file.close();
    }
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__;
    qDebug() << "DEBUG:" << replyStr;
#endif

    QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());                
    if (!doc.isObject()) {
        Q_EMIT error();
        return;
    }
    QJsonObject obj = doc.object();
    QJsonObject user = obj["User"].toObject();
    m_loginUserName = user["UserName"].toString();
    m_loginNickName = user["NickName"].toString();
#if QWX_DEBUG
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << m_loginUserName;
#endif
    Q_EMIT loginUserNameChanged();
    Q_EMIT loginHeadImgUrlChanged();

    Q_FOREACH (const QJsonValue & val, obj["ContactList"].toArray()) {
        QJsonObject user = val.toObject();
        QString userName = user["UserName"].toString();
        QString nickName = user["NickName"].toString();
        if (!m_map.contains(userName)) {
            m_contactList.append(new UserObject(
                userName, 
                nickName, 
                m_v2 ? WX_V2_SERVER_HOST + user["HeadImgUrl"].toString() : WX_SERVER_HOST + user["HeadImgUrl"].toString()));
        }
        m_map.insert(userName, nickName);
    }
    Q_EMIT contactListChanged();

    QString skey = obj["SKey"].toString();                                         
#if QWX_DEBUG                                                                      
    qDebug() << "DEBUG:" << __PRETTY_FUNCTION__ << skey;                           
#endif 
    QStringList syncKey;
    Q_FOREACH (const QJsonValue & val, obj["SyncKey"].toObject()["List"].toArray()) {
        syncKey.append(QString::number(val.toObject()["Key"].toInt()) + "|" + 
                QString::number(val.toObject()["Val"].toInt()));
    }
    Q_EMIT skeyChanged(skey, syncKey);
}
