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
        PersonRepository() { setSort(utils::IDASC); }
        PersonRepository(utils::SORTS s)
            : sortOrder(s) { }

        bool      addPerson (const Person  p);
        QSqlError getPerson (uint id, Person &p);
        QSqlError editPerson(const Person newInfo);

        QSqlError findSimilar(QString expr, QVector<Person> &pList);

        QVector<Person> getAll();

        void setSort(utils::SORTS s) { sortOrder = s; }
        utils::SORTS getSortOrder()  { return sortOrder; }
    private:
        QSqlDatabase db;
        utils::SORTS sortOrder;

        void getIndexes(int &indexI, int &indexN, int &indexG,
                        int &indexB, int &indexD, QSqlQuery q);

        string getOrderBy();
        QSqlError getConnectedComps(uint id, QVector<uint> &connectedComps);
};

#endif // PERSONREPOSITORY_H
