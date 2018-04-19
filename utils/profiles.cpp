﻿#include "profiles.h"
#include "hqutils.h"
#include <QDir>
#include <QDebug>

#define     INI_FILE_NAME           "profiles.ini"

Profiles* Profiles::minstance = 0;
Profiles::MGarbage Profiles::Garbage;
Profiles::Profiles(QObject *parent) :
    configSettings(NULL),
    QObject(parent)
{
    createFile();
    configSettings = new QSettings(INI_FILE_NAME, QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
    qDebug()<<__func__<<configSettings->allKeys()<<configSettings->childGroups();
}

void Profiles::createFile()
{
    qDebug()<<__func__<<__LINE__;
    if(!QFile::exists(INI_FILE_NAME))
    {
        qDebug()<<__func__<<__LINE__;
        QFile fp(INI_FILE_NAME);
        bool sts = fp.open(QIODevice::ReadWrite);
        qDebug()<<sts;
        if(sts)
        {
            qDebug()<<__func__<<__LINE__;
            fp.write("[DateManage]\n\
                    Expired=2017-01-01, 2017-01-02, 2017-01-03, 2017-01-27, 2017-01-28, 2017-01-29, 2017-01-30, 2017-01-31, 2017-02-01, 2017-02-02, 2017-04-02, 2017-04-03, 2017-04-04, 2017-04-29, 2017-04-30, 2017-05-01, 2017-05-28, 2017-05-29, 2017-05-30, 2017-10-01, 2017-10-02, 2017-10-03, 2017-10-04, 2017-10-05, 2017-10-06, 2017-10-07, 2017-10-08, 2018-01-01, 2018-02-15, 2018-02-16, 2018-02-17, 2018-02-18, 2018-02-19, 2018-02-20, 2018-02-21, 2018-04-05, 2018-04-06, 2018-04-07, 2018-04-29, 2018-04-30, 2018-05-01, 2018-06-16, 2018-06-17, 2018-06-18, 2018-09-22, 2018-09-23, 2018-09-24, 2018-10-01, 2018-10-02, 2018-10-03, 2018-10-04, 2018-10-05, 2018-10-06, 2018-10-07\n\
                    [UPDATE]\n\
                    DATE=2018-03-30\n\
                    [ZXG]\n\
                    codes=sz300433, sz300676, sz300059, sz300015, sz300124, sz300072, sz300003, sz300122, sz300070, sz300296, sz300136, sz300251, sz300618, sz300017, sz300418, sz300450, sz300142, sz300024, sh601398, sh601939, sh601857, sh601318, sh601288, sh601988, sh600519, sh600036, sh600028, sh601628, sh601088, sh600104, sz002415, sz000333, sh601601, sz000002, sz000651, sh601360, sh601766, sz000858, sh601668, sh600276, sh600030, sz002352, sh603993, sh600019, sz000001, sz000725, sh600887, sh600050, sh601111, sz002027, sh600585, sz001979, sz002594, sz002304, sh600048, sh601336, sh603288, sz000063, sz000776, sh601688, sz002024, sh600690, sh600196, sh600029, sh601888, sh600703, sz000538, sh601899, sh600115, sh600340, sh601881, sh601933, sz002142, sh600309, sh600009, sz000895, sz000568, sz002230, sh601155, sz002475, sz002236, sh601225, sh601600, sh603799, sz000938, sh601012, sh601607, sz002466, sh600188, sz002202, sh600660, sz002044, sz002558, sh600031, sh601919, sh600362, sz002008, sz002456, sz002460, sh600893, sz002600, sh600383, sh600588, sh601992, sh600809, sh600436, sh600176, sh600111, sz002241, sh600547, sh600516, sh600332, sz000898, sh603019, sh600570, sh603160, sz002405, sh601699\n");
            fp.close();
                    qDebug()<<__func__<<__LINE__;
        }
        qDebug()<<__func__<<__LINE__;
    }
    qDebug()<<__func__<<__LINE__;
}

Profiles::~Profiles()
{
    if(configSettings)
    {
        configSettings->deleteLater();
    }
//    if ( minstance )
//    {
//        delete minstance;
//        minstance = 0;
//    }
}
/*-------------------------------------------
 *
 * 实例化
 *
---------------------------------------------*/
Profiles *Profiles::instance()
{
    if ( minstance == 0)
    {
        minstance = new Profiles();
    }
    return minstance;
}
/*-------------------------------------------
 *
 * 设置默认值
 *
---------------------------------------------*/
void Profiles::setDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    configSettings->beginGroup(prefix);
    if(configSettings->value(key).toString().isEmpty())
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 *
 * 设置配置文件值
 *
---------------------------------------------*/
void Profiles::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
//    qDebug()<<"prefix:"<<prefix<<" key:"<<key<<" value:"<<value;
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 *
 * 返回值
 *
---------------------------------------------*/
QVariant Profiles::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    configSettings->beginGroup(prefix);
    {
        values =  configSettings->value( keys,defaultValue);
    }
    configSettings->endGroup();
    return values;
}

QStringList Profiles::getAllSections()
{
    qDebug()<<__func__<<QFile::exists(INI_FILE_NAME)<<configSettings->fileName();
    return configSettings->childGroups();
}
