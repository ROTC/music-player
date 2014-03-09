#ifndef DBMODULE_H
#define DBMODULE_H
#include <QObject>
#include <QStringList>

class DBModule : public QObject
{
    Q_OBJECT

public:
    explicit DBModule();

    static void InitDB();
    static bool CreateConnection();
    static void CreateTable();
    static bool ClearDB();
    static bool DeleteLine(const QString &filename);
    static bool InsertLine(const QString &filename);
    static bool ReadFromDb(QStringList &list);				//从数据库中读出数据显示到音乐里列表中
};

#endif // DBMODULE_H
