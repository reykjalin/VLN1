#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include <algorithm>

#include "Models/person.h"
#include "Models/computer.h"
#include "DataAccess/dataaccess.h"
#include "DataAccess/personrepository.h"
#include "DataAccess/computerrepository.h"

using namespace std;

class PersonService
{
    public:
        PersonService() { }

        /**
         * @brief getPersonList - get list of Persons in database
         * @return List containing person information
         */
        QVector<Person> getPersonList() { return db2.getAll(); }
        QVector<Computer> getComputerList() { return cRepo.getAll(); }
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
        bool startService() { return db.readDataFromDB() && !db2.initRepo().isValid(); }

        /**
         * @brief addPerson - Add person to DB
         * @param p Person to add
         * @return true if successful, false otherwise
         */
        bool addPerson(Person p) { return db2.addPerson(p); }
        /**
         * @brief getPerson - Get person from DB
         * @param index Index of person to fetch
         * @param p Person object to store information
         * @return true if successful, false otherwise
         */
        bool getPerson(unsigned int index, Person &p) { return !db2.getPerson(index, p).isValid(); }
        /**
         * @brief editPerson - Edit person in DB
         * @param index Index of person to edit
         * @param newInfo Person object containing new information
         * @return
         */
        QSqlError editPerson(Person newInfo) { return db2.editPerson(newInfo); }

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
        PersonRepository db2;
        ComputerRepository cRepo;
};

#endif // PERSONSERVICE_H
