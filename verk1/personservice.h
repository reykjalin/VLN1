#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include <algorithm>

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

        void sortName()   { stable_sort(pList.begin(), pList.end(), utils::sortName); }
        void sortGender() { stable_sort(pList.begin(), pList.end(), utils::sortGender); }
        void sortBirth()  { stable_sort(pList.begin(), pList.end(), utils::sortBirth); }
        void sortDeath()  { stable_sort(pList.begin(), pList.end(), utils::sortDeath); }

    private:
        QVector<Person> pList;
        DataAccess db;
};

#endif // PERSONSERVICE_H
