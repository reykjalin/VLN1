#include "computerrepository.h"

QSqlError ComputerRepository::initRepo() {
    return utils::initDB(db);
}

QSqlError ComputerRepository::addComputer(const Computer c) {
    QSqlQuery q;
    q.prepare(QLatin1String("INSERT INTO computers (name, type, ") +
              QLatin1String("year_built, built) ") +
              QLatin1String("VALUES (:name, :type, :year_built, :built)"));
    q.bindValue(":name", c.getName());
    q.bindValue(":type", c.getType());
    q.bindValue(":year_built", c.getYearBuilt());
    q.bindValue(":built", c.getBuilt());

    if(q.exec())
        return QSqlError();
    return q.lastError();
}

QSqlError ComputerRepository::editComputer(const Computer newInfo) {
    QSqlQuery q;
    q.prepare(QLatin1String("UPDATE computers SET ")      +
              QLatin1String("name=(:name), ")             +
              QLatin1String("type=(:type), ")             +
              QLatin1String("year_built=(:year_built), ") +
              QLatin1String("built=(:built) ")            +
              QLatin1String("WHERE id=(:id)"));
    q.bindValue(":name", newInfo.getName());
    q.bindValue(":type", newInfo.getType());
    q.bindValue(":year_built", newInfo.getYearBuilt());
    q.bindValue(":built", newInfo.getBuilt());
    q.bindValue(":id", newInfo.getId());

    if(!q.exec())
        return q.lastError();

    return QSqlError();
}

QVector<Computer> ComputerRepository::getAll() {
    // TODO: Error handling
    QVector<Computer> cList;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM computers"));
    if(!q.exec()) {
        qDebug() << q.lastError();
        return QVector<Computer>();
    }
    // Get indexes for columns in DB
    int indexI, indexN, indexT, indexY, indexB;
    getIndexes(q, indexI, indexN, indexT, indexY, indexB);

    // Iterate through results from query, build person object and add to list
    while(q.next()) {
        uint    id    = q.value(indexI).toUInt();
        QString name  = q.value(indexN).toString();
        QString type  = q.value(indexT).toString();
        int     yearb = q.value(indexY).toInt();
        bool    built = q.value(indexB).toBool();

        cList.append(Computer(name, type, yearb, built, id));
    }

    return cList;
}

QSqlError ComputerRepository::getComputer(uint id, Computer &c) {
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM computers WHERE id=(:id)"));
    q.bindValue(":id", id);
    if(!q.exec()) {
        return q.lastError();
    }

    // Get indexes for columns in DB
    int indexI, indexN, indexT, indexY, indexB;
    getIndexes(q, indexI, indexN, indexT, indexY, indexB);

    // q.next() == true    =>    got result   =>   person exists
    if(q.next()) {
        c.setId       (q.value(indexI).toUInt());
        c.setName     (q.value(indexN).toString());
        c.setType     (q.value(indexT).toString());
        c.setYearBuilt(q.value(indexY).toInt());
        c.setBuilt    (q.value(indexB).toBool());
        return QSqlError();
    }

    return QSqlError("", "Computer not found", QSqlError::TransactionError);
}

void ComputerRepository::getIndexes(QSqlQuery q, int &indexI, int &indexN,
                                    int &indexT, int &indexY, int &indexB)
{
    indexI = q.record().indexOf("id");
    indexN = q.record().indexOf("name");
    indexT = q.record().indexOf("type");
    indexY = q.record().indexOf("year_built");
    indexB = q.record().indexOf("built");
}
