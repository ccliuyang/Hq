﻿#include "hqdbdatabase.h"
#include <QMutexLocker>
#include <QDebug>

#define     QDebug()            qDebug()<<__FUNCTION__<<__LINE__

HQDBDataBase::HQDBDataBase(QObject *parent) : QObject(parent)
{
    mInitDBFlg = initSqlDB();
    if(mInitDBFlg)
    {
        mSQLQuery = QSqlQuery(mDB);
    }
}

HQDBDataBase::~HQDBDataBase()
{
    if(mInitDBFlg)
    {
        mDB.close();
    }
}

bool HQDBDataBase::isDBOK()
{
    return mInitDBFlg;
}

bool HQDBDataBase::initSqlDB()
{
    //初始化本地数据库的连接
    qDebug()<<"database:"<<QSqlDatabase::database().databaseName();
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        mDB = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        mDB = QSqlDatabase::addDatabase("QSQLITE");
    }
    mDB.setDatabaseName("StockData.s3db");
    return mDB.open();
}

QString HQDBDataBase::getErrorString()
{
    QMutexLocker locker(&mSQlMutex);
    return QString("Error Content: \n %1 \n %2").arg(mSQLQuery.lastQuery()).arg(mSQLQuery.lastError().text());
}

bool HQDBDataBase::isTableExist(const QString &pTable)
{
    QMutexLocker locker(&mSQlMutex);
    if(!mSQLQuery.exec(tr("SELECT COUNT(*) FROM sqlite_master where type='table' and name='%1'").arg(pTable)))
    while (mSQLQuery.next()) {
        return mSQLQuery.value(0).toBool();
    }
    return false;
}

bool HQDBDataBase::createTable(const QString &pTable, const QMap<QString, QString> &cols)
{
    QStringList colist;
    foreach (QString key, cols.keys()) {
        colist.append(tr(" [%1] %2 ").arg(key).arg(cols[key]));
    }

    QString sql = tr("CREATE TABLE [%1] ( %2 )").arg(pTable).arg(colist.join(","));
    QMutexLocker locker(&mSQlMutex);
    return mSQLQuery.exec(sql);
}

QDate HQDBDataBase::getLastUpdateDateOfTable(const QString &table)
{
    QMutexLocker locker(&mSQlMutex);
    QDate date = QDate(2016, 12, 4);
    if(mSQLQuery.exec(tr("select max(date) from %1").arg(table)))
    {
        while (mSQLQuery.next()) {
            if(!mSQLQuery.value(0).isNull())
            {
                date = mSQLQuery.value(0).toDate();
                break;
            }
        }
    }
    return date;
}

bool HQDBDataBase::getBlockDataList(BlockDataList &list, int type)
{
    QString filter = (type != 0 ? tr(" where %1 = %2").arg(HQ_TABLE_COL_BLOCK_TYPE).arg(type) : "");
//    if(!mSqlQuery.exec(tr("select * from Block %1").arg(filter))) return;

#if 0
    QList<BlockRealInfo> selist;
    while (mSqlQuery.next()) {
        BlockRealInfo info;
        int index = 0;
        info.mCode = mSqlQuery.value(index++).toInt();
        info.mName = mSqlQuery.value(index++).toString();
        info.mCurPrice = mSqlQuery.value(index++).toDouble();
        info.mChange = mSqlQuery.value(index++).toDouble();
        info.mChangePercent = mSqlQuery.value(index++).toDouble();
        info.mZjlx = mSqlQuery.value(index++).toDouble();
        info.mShareCodesList = mSqlQuery.value(index++).toStringList();
        info.mType = mSqlQuery.value(index++).toInt();
        info.mDate = QDateTime::fromMSecsSinceEpoch(mSqlQuery.value(index++).toLongLong()).date();
        if(init) mBlockInfo[info.mCode] = info;
        selist.append(info);
    }

    emit signalSendBlockInfoList(selist);
#endif
    return true;
}

