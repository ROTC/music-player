#include "DBModule.h"
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

DBModule::DBModule()
{

}

void DBModule::InitDB()
{
    bool exist = QFile::exists("rotc.sqlite");

    if(!DBModule::CreateConnection())
    {
        QMessageBox::warning(0, tr("提示"), tr("连接数据库错误!"));
        return;
    }

    if(exist)
    {
        DBModule::CreateTable();
    }
}

bool DBModule::CreateConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("rotc.sqlite");//若数据库文件不存在，则自动创建一个
    if (!db.open())
    {
        QMessageBox::warning(0, tr("数据库打开错误!"),
            db.lastError().databaseText());
        return false;
    }
    return true;
}

void DBModule::CreateTable()
{
    QSqlQuery query;
    query.exec("drop table musicList");

    query.exec("create table musicList (artist varchar(15),fileName varchar(50) primary key not null, "
        "album varchar(50),filepath varchar(50))");
}

bool DBModule::ReadFromDb(QStringList &list)
{
    QSqlQuery query;
    query.exec("select filepath from musiclist");
    while(query.next())
    {
        QString filepath = query.value(0).toString();
        list << filepath;
    }
    if(!list.empty())
        return true;
    return false;
}

bool DBModule::ClearDB()
{
    QSqlQuery query;
    return query.exec("delete from musiclist");
}

bool DBModule::DeleteLine(const QString &filename)
{
    QSqlQuery query;
    query.prepare("delete from musiclist wherer filename = ?");
    query.bindValue(filename,QVariant::String);
    return query.exec();
}

bool DBModule::InsertLine(const QString& artist,const QString& filename,const QString& album,const QString& filepath)
{
    QSqlQuery query;
    query.prepare("insert into musiclist values(?,?,?,?)");
    query.bindValue(artist);
    query.bindValue(filename);
    query.bindValue(album);
    query.bindValue(filepath);
    return query.exec();
}
