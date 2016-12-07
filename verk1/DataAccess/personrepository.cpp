#include "personrepository.h"

QSqlError PersonRepository::initRepo() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mainDB");
    if(!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if(!tables.contains("computer_scientists")) {
        QSqlQuery q;
        cout << "executing create" << endl;
        if(!q.exec(QLatin1String("CREATE TABLE computer_scientists(") +
                   QLatin1String("id integer primary key,") +
                   QLatin1String("name varchar,") +
                   QLatin1String("gender varcahr,") +
                   QLatin1String("birth_year integer,") +
                   QLatin1String("death_year integer") +
                   QLatin1String(")"))) return db.lastError();
    }

    return QSqlError();
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

QVector<Person> PersonRepository::getAll() {
    // TODO: Error handling
    QVector<Person> pList;

    QSqlQuery q(QLatin1String("SELECT * FROM computer_scientists"));
    int indexI, indexN, indexG, indexB, indexD;
    getIndexes(indexI, indexN, indexG, indexB, indexD, q);

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

    if(q.next()) {
        p.setId       (q.value(indexI).toUInt());
        p.setName     (q.value(indexN).toString());
        p.setGender   (q.value(indexG).toString());
        p.setBirthYear(q.value(indexB).toInt());
        p.setDeathYear(q.value(indexD).toInt());
        return QSqlError();
    }
    return q.lastError();
}

QSqlError PersonRepository::getIndexes(int &indexI, int &indexN, int &indexG,
                                       int &indexB, int &indexD, QSqlQuery q)
{
    if(!q.isValid())
        return q.lastError();
    indexI = q.record().indexOf("id");
    indexN = q.record().indexOf("name");
    indexG = q.record().indexOf("gender");
    indexB = q.record().indexOf("birth_year");
    indexD = q.record().indexOf("death_year");
    return QSqlError();
}
