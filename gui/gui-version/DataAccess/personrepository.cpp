#include "personrepository.h"

bool PersonRepository::addPerson(const Person p) {
    QSqlQuery q;
    q.prepare(QLatin1String("INSERT INTO computer_scientists (name, gender, ") +
              QLatin1String("birth_year, death_year) ") +
              QLatin1String("VALUES (:name, :gender, :birth_year, :death_year)"));
    q.bindValue(":name", p.getName());
    q.bindValue(":gender", p.getGender());
    q.bindValue(":birth_year", p.getBirthYear());
    q.bindValue(":death_year", p.getDeathYear());

    if(q.exec())
        return true;
    qDebug() << q.lastError();
    return false;
}

QSqlError PersonRepository::editPerson(const Person newInfo) {
    QSqlQuery q;
    q.prepare(QLatin1String("UPDATE computer_scientists SET ") +
              QLatin1String("name=(:name), ")                  +
              QLatin1String("gender=(:gender), ")              +
              QLatin1String("birth_year=(:birth_year), ")      +
              QLatin1String("death_year=(:death_year) ")       +
              QLatin1String("WHERE id=(:id)"));
    q.bindValue(":name", newInfo.getName());
    q.bindValue(":gender", newInfo.getGender());
    q.bindValue(":birth_year", newInfo.getBirthYear());
    q.bindValue(":death_year", newInfo.getDeathYear());
    q.bindValue(":id", newInfo.getId());

    if(!q.exec())
        return q.lastError();

    q.prepare(QLatin1String("DELETE FROM scientists_and_computers WHERE scientist_id=(:sid)"));
    q.bindValue(":sid", newInfo.getId());

    if(!q.exec())
        return q.lastError();

    for(int i = 0; i < newInfo.getConns().length(); i++) {
        q.prepare(QLatin1String("INSERT OR IGNORE INTO scientists_and_computers ") +
                  QLatin1String("(scientist_id, computer_id) VALUES ")             +
                  QLatin1String("(:sid, :cid)"));
        q.bindValue(":sid", newInfo.getId());
        q.bindValue(":cid", newInfo.getConns()[i]);

        if(!q.exec())
            return q.lastError();
    }

    return QSqlError();
}

QVector<Person> PersonRepository::getAll() {
    // TODO: Error handling
    QVector<Person> pList;

    QSqlQuery q;
    string queryStr = "SELECT * FROM computer_scientists " + getOrderBy();
    q.prepare(QLatin1String(queryStr.data()));
    if(!q.exec()) {
        qDebug() << q.lastError();
        return QVector<Person>();
    }
    // Get indexes for columns in DB
    int indexI, indexN, indexG, indexB, indexD;
    getIndexes(indexI, indexN, indexG, indexB, indexD, q);

    // Iterate through results from query, build person object and add to list
    while(q.next()) {
        uint id        = q.value(indexI).toUInt();
        QString name   = q.value(indexN).toString();
        QString gender = q.value(indexG).toString();
        int byear      = q.value(indexB).toInt();
        int dyear      = q.value(indexD).toInt();
        QVector<uint> connComps;
        if(getConnectedComps(id, connComps).isValid())
            qDebug() << "Error occurred when getting connected computers for personID = "
                     << id << endl;

        pList.append(Person(name, gender, byear, dyear, connComps, id));
    }
    return pList;
}

string PersonRepository::getOrderBy() {
    switch(sortOrder) {
        case utils::IDASC:                return "ORDER BY id ASC";             break;
        case utils::IDDESC:               return "ORDER BY id DESC";            break;
        case utils::NAMEASC:              return "ORDER BY LOWER(name) ASC";    break;
        case utils::NAMEDESC:             return "ORDER BY LOWER(name) DESC";   break;
        case utils::GENDER_TYPE_ASC:      return "ORDER BY LOWER(gender) ASC";  break;
        case utils::GENDER_TYPE_DESC:     return "ORDER BY LOWER(gender) DESC"; break;
        case utils::DEATH_BUILT_ASC:      return "ORDER BY death_year ASC";     break;
        case utils::DEATH_BUILT_DESC:     return "ORDER BY death_year DESC";    break;
        case utils::BIRTH_BUILDYEAR_ASC:  return "ORDER BY birth_year ASC";     break;
        case utils::BIRTH_BUILDYEAR_DESC: return "ORDER BY birth_year DESC";    break;
        default:                          return "";                            break;
    }
}

QSqlError PersonRepository::getPerson(uint id, Person &p) {
    QSqlQuery q;
    q.prepare("SELECT * FROM computer_scientists where id=(:id)");
    q.bindValue(":id", id);
    if(!q.exec())
        return q.lastError();

    int indexI, indexN, indexG, indexB, indexD;
    getIndexes(indexI, indexN, indexG, indexB, indexD, q);

    // q.next() == true    =>    got result   =>   person exists
    if(q.next()) {
        p.setId       (q.value(indexI).toUInt());
        p.setName     (q.value(indexN).toString());
        p.setGender   (q.value(indexG).toString());
        p.setBirthYear(q.value(indexB).toInt());
        p.setDeathYear(q.value(indexD).toInt());

        QVector<uint> conns;
        QSqlError e = getConnectedComps(id, conns);
        if(e.isValid())
            return e;
        p.setConns(conns);

        return QSqlError();
    }
    return QSqlError("", "Person not found", QSqlError::TransactionError);
}

QSqlError PersonRepository::getConnectedComps(uint id, QVector<uint> &connectedComps) {
    QSqlQuery q;
    q.prepare("SELECT computer_id FROM scientists_and_computers WHERE scientist_id=(:id)");
    q.bindValue(":id", id);

    if(!q.exec())
        return q.lastError();

    while(q.next())
        connectedComps.append(q.value(0).toUInt());

    return QSqlError();
}

QSqlError PersonRepository::findSimilar(QString expr, QVector<Person> &pList) {
    QSqlQuery q;
    expr.prepend("%");
    expr.append("%");
    q.prepare(QLatin1String("SELECT * FROM computer_scientists WHERE ") +
              QLatin1String("id LIKE '%?%' OR ")                        +
              QLatin1String("name LIKE (:expr2) OR ")                   +
              QLatin1String("gender LIKE (:expr3) OR ")                 +
              QLatin1String("birth_year LIKE (:expr4) OR ")             +
              QLatin1String("death_year LIKE (:expr5) ")                +
              QLatin1String(getOrderBy().data()));
    q.bindValue(":expr1", expr);
    q.bindValue(":expr2", expr);
    q.bindValue(":expr3", expr);
    q.bindValue(":expr4", expr);
    q.bindValue(":expr5", expr);

    qDebug() << getOrderBy().data() << endl;

    if(!q.exec())
        return q.lastError();

    int indexI, indexN, indexG, indexB, indexD;
    getIndexes(indexI, indexN, indexG, indexB, indexD, q);

    // Iterate through results from query, build person object and add to list
    while(q.next()) {
        uint id        = q.value(indexI).toUInt();
        QString name   = q.value(indexN).toString();
        QString gender = q.value(indexG).toString();
        int byear      = q.value(indexB).toInt();
        int dyear      = q.value(indexD).toInt();
        QVector<uint> connComps;
        if(getConnectedComps(id, connComps).isValid())
            qDebug() << "Error occurred when getting connected computers for personID = "
                     << id << endl;

        pList.append(Person(name, gender, byear, dyear, connComps, id));
    }
    return QSqlError();
}

void PersonRepository::getIndexes(int &indexI, int &indexN, int &indexG,
                                  int &indexB, int &indexD, QSqlQuery q)
{
    indexI = q.record().indexOf("id");
    indexN = q.record().indexOf("name");
    indexG = q.record().indexOf("gender");
    indexB = q.record().indexOf("birth_year");
    indexD = q.record().indexOf("death_year");
}
