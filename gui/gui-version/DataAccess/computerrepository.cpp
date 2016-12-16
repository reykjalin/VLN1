#include "computerrepository.h"

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

    q.prepare(QLatin1String("DELETE FROM scientists_and_computers WHERE computer_id=(:cid)"));
    q.bindValue(":cid", newInfo.getId());

    if(!q.exec())
        return q.lastError();

    for(int i = 0; i < newInfo.getConns().length(); i++) {
        q.prepare(QLatin1String("INSERT OR IGNORE INTO scientists_and_computers ") +
                  QLatin1String("(scientist_id, computer_id) VALUES ")             +
                  QLatin1String("(:sid, :cid)"));
        q.bindValue(":sid", newInfo.getConns()[i]);
        q.bindValue(":cid", newInfo.getId());

        if(!q.exec())
            return q.lastError();
    }

    return QSqlError();
}

QVector<Computer> ComputerRepository::getAll() {
    // TODO: Error handling
    QVector<Computer> cList;

    QSqlQuery q;
    string queryStr = "SELECT * FROM computers ";
    queryStr += getOrderBy();
    q.prepare(QLatin1String(queryStr.data()));

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

        QVector<uint> conns;
        if(getConnectedPersons(id, conns).isValid())
            qDebug() << "Error occurred while getting connected individuals for computerID = "
                     << id << endl;

        cList.append(Computer(name, type, yearb, built, conns, id));
    }

    return cList;
}

string ComputerRepository::getOrderBy() {
    switch(sortOrder) {
        case utils::IDASC:                return "ORDER BY id ASC";           break;
        case utils::IDDESC:               return "ORDER BY id DESC";          break;
        case utils::NAMEASC:              return "ORDER BY LOWER(name) ASC";  break;
        case utils::NAMEDESC:             return "ORDER BY LOWER(name) DESC"; break;
        case utils::GENDER_TYPE_ASC:      return "ORDER BY LOWER(type) ASC";  break;
        case utils::GENDER_TYPE_DESC:     return "ORDER BY LOWER(type) DESC"; break;
        case utils::DEATH_BUILT_ASC:      return "ORDER BY built ASC";        break;
        case utils::DEATH_BUILT_DESC:     return "ORDER BY built DESC";       break;
        case utils::BIRTH_BUILDYEAR_ASC:  return "ORDER BY year_built ASC";   break;
        case utils::BIRTH_BUILDYEAR_DESC: return "ORDER BY year_built DESC";  break;
        default:                          return "";                          break;
    }
}

QSqlError ComputerRepository::findSimilar(QString expr, QVector<Computer> &cList) {
    QSqlQuery q;
    expr.prepend("%");
    expr.append("%");
    q.prepare(QLatin1String("SELECT * FROM computers WHERE ") +
              QLatin1String("id LIKE '%?%' OR ")              +
              QLatin1String("name LIKE (:expr2) OR ")         +
              QLatin1String("type LIKE (:expr3) OR ")         +
              QLatin1String("year_built LIKE (:expr4) OR ")   +
              QLatin1String("built LIKE (:expr5) ")           +
              QLatin1String(getOrderBy().data()));
    q.bindValue(":expr1", expr);
    q.bindValue(":expr2", expr);
    q.bindValue(":expr3", expr);
    q.bindValue(":expr4", expr);
    q.bindValue(":expr5", expr);

    if(!q.exec())
        return q.lastError();

    int indexI, indexN, indexT, indexY, indexB;
    getIndexes(q, indexI, indexN, indexT, indexY, indexB);

    // Iterate through results from query, build person object and add to list
    while(q.next()) {
        uint id      = q.value(indexI).toUInt();
        QString name = q.value(indexN).toString();
        QString type = q.value(indexT).toString();
        int byear    = q.value(indexY).toInt();
        bool built   = q.value(indexB).toBool();

        QVector<uint> conns;
        QSqlError e = getConnectedPersons(id, conns);
        if(e.isValid())
            return e;

        cList.append(Computer(name, type, byear, built, conns, id));
    }
    return QSqlError();
}

QSqlError ComputerRepository::getComputer(Computer &c) {
    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM computers WHERE id=(:id)"));
    q.bindValue(":id", c.getId());
    if(!q.exec()) {
        return q.lastError();
    }

    // Get indexes for columns in DB
    int indexI, indexN, indexT, indexY, indexB;
    getIndexes(q, indexI, indexN, indexT, indexY, indexB);

    // q.next() == true    =>    got result   =>   person exists
    if(q.next()) {
        c.setName     (q.value(indexN).toString());
        c.setType     (q.value(indexT).toString());
        c.setYearBuilt(q.value(indexY).toInt());
        c.setBuilt    (q.value(indexB).toBool());

        QVector<uint> conns;
        QSqlError e = getConnectedPersons(c.getId(), conns);
        if(e.isValid())
            return e;
        c.setConns(conns);

        return QSqlError();
    }

    return QSqlError("", "Computer not found", QSqlError::TransactionError);
}

QSqlError ComputerRepository::getConnectedPersons(uint id, QVector<uint> &connectedPersons) {
    QSqlQuery q;
    q.prepare("SELECT scientist_id FROM scientists_and_computers WHERE computer_id=(:id)");
    q.bindValue(":id", id);

    if(!q.exec())
        return q.lastError();

    while(q.next())
        connectedPersons.append(q.value(0).toUInt());

    return QSqlError();
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
