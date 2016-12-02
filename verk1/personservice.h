#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include "person.h"
#include "dataaccess.h"

using namespace std;

class PersonService
{
    public:
        PersonService() { }

        QVector<Person> getPersonList() { return pList; }
        QVector<Person> findSimilar(QString expr);

        bool closeService() { return db.saveData(pList); }
        bool startService() { return db.readDataFromDB(pList); }
        bool addPerson(Person p);

    private:
        QVector<Person> pList;
        DataAccess db;

};

#endif // PERSONSERVICE_H
