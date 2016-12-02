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

        /**
         * @brief getPersonList - get list of Persons in database
         * @return List containing person information
         */
        QVector<Person> getPersonList() { return db.getPersonList(); }
        /**
         * @brief findSimilar - search through DB for something simalar to expr
         * @param expr Query to search for
         * @return List of similar entries
         */
        QVector<Person> findSimilar(QString expr);

        /**
         * @brief closeService - close service and save information
         * @return true if successful, false otherwise
         */
        bool closeService() { return db.saveData(); }
        /**
         * @brief startService - start service; load information
         * @return true if successful, false otherwise
         */
        bool startService() { return db.readDataFromDB(); }
        /**
         * @brief addPerson - Add person to DB
         * @param p Person to add
         * @return true if successful, false otherwise
         */
        bool addPerson(Person p) { return db.addPerson(p); }
        /**
         * @brief loadDataFromFile - Load file into DB
         * @param fname File to load into DB
         * @return true if successful, false otherwise
         */
        bool loadDataFromFile(QString fname) { return db.importFromFile(fname); }

        /**
         * @brief setSort - Set how sortOrder for sorting
         * @param s Sort order for list of person objects
         */
        void setSort(utils::SORTS s) { db.setSort(s); }
        /**
         * @brief sort - sort list of person objects
         */
        void sort() { db.sort(); }

    private:
        /**
         * @brief db - Database connection
         */
        DataAccess db;
};

#endif // PERSONSERVICE_H
