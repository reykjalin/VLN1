#include "personrepository.h"

QSqlError PersonRepository::initRepo() {
    return utils::initDB(db);
}

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

    return QSqlError();
}

QVector<Person> PersonRepository::getAll() {
    // TODO: Error handling
    QVector<Person> pList;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM computer_scientists"));
    if(!q.exec()) {
        qDebug() << q.lastError();
        return QVector<Person>();
    }
    // Get indexes for columns in DB
    int indexI, indexN, indexG, indexB, indexD;
    getIndexes(indexI, indexN, indexG, indexB, indexD, q);

    // Iterate through results from query, build person object and add to list
    while(q.next()) {
        unsigned int id = q.value(indexI).toUInt();
        QString name    = q.value(indexN).toString();
        QString gender  = q.value(indexG).toString();
        int byear       = q.value(indexB).toInt();
        int dyear       = q.value(indexD).toInt();

        pList.append(Person(name, gender, byear, dyear, id));
    }

    return pList;
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
        return QSqlError();
    }
    return QSqlError("", "Person not found", QSqlError::TransactionError);
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
