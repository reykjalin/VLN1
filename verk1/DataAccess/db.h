#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>

class DB
{
    public:
        DB() { }

        QSqlError init();

    private:
        QSqlDatabase db;
};

#endif // DB_H
