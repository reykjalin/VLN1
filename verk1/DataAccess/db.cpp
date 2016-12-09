#include "db.h"

QSqlError DB::init() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mainDB");
    if(!db.open())
        return db.lastError();

    // Create tables if they don't exist
    QStringList tables = db.tables();
    if(!tables.contains("computer_scientists")) {
        QSqlQuery q;
        if(!q.exec(QLatin1String("CREATE TABLE computer_scientists(") +
                   QLatin1String("id integer primary key,") +
                   QLatin1String("name varchar,") +
                   QLatin1String("gender varcahr,") +
                   QLatin1String("birth_year integer,") +
                   QLatin1String("death_year integer") +
                   QLatin1String(")"))) return q.lastError();
    }

    if(!tables.contains("computers")) {
        QSqlQuery q;
        if(!q.exec(QLatin1String("CREATE TABLE computers(") +
                   QLatin1String("id integer primary key,") +
                   QLatin1String("name varchar,") +
                   QLatin1String("type varchar,") +
                   QLatin1String("year_built integer,") +
                   QLatin1String("built integer") +
                   QLatin1String(")"))) return q.lastError();
    }

    if(!tables.contains("scientists_and_computers")) {
        QSqlQuery q;
        if(!q.exec(QLatin1String("CREATE TABLE scientists_and_computers(") +
                   QLatin1String("scientist_id integer,") +
                   QLatin1String("computer_id integer,") +
                   QLatin1String("PRIMARY KEY (scientist_id, computer_id), ") +
                   QLatin1String("FOREIGN KEY (scientist_id) REFERENCES computer_scientists(id) ") +
                   QLatin1String("ON DELETE CASCADE ON UPDATE NO ACTION, ") +
                   QLatin1String("FOREIGN KEY (computer_id) REFERENCES computers(id) ") +
                   QLatin1String("ON DELETE CASCADE ON UPDATE NO ACTION ") +
                   QLatin1String(")"))) return q.lastError();
    }

    return QSqlError();
}
