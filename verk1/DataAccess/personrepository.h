#ifndef PERSONREPOSITORY_H
#define PERSONREPOSITORY_H

#include <QtSql>
#include <QStringList>
#include <QVector>
#include <QString>

#include "Models/person.h"
#include "Utilities/utils.h"

class PersonRepository
{
    public:
        PersonRepository() { }

        QSqlError initRepo();

        bool      addPerson (const Person  p);
        QSqlError getPerson (uint id, Person &p);
        QSqlError editPerson(const Person newInfo);

        QVector<Person> getAll();
    private:
        QSqlDatabase db;

        void getIndexes(int &indexI, int &indexN, int &indexG,
                        int &indexB, int &indexD, QSqlQuery q);

};

#endif // PERSONREPOSITORY_H
