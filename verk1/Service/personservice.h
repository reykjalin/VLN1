#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include <algorithm>

#include "Models/person.h"
#include "Models/computer.h"
#include "DataAccess/dataaccess.h"
#include "DataAccess/personrepository.h"
#include "DataAccess/computerrepository.h"
#include "DataAccess/db.h"

using namespace std;

class PersonService
{
    public:
        PersonService() { }

        /**
         * @brief getPersonList - get list of Persons in database
         * @return List containing person information
         */
        QVector<Person>   getPersonList();
        QVector<Computer> getComputerList() { return cRepo.getAll(); }
        /**
         * @brief findSimilar - search through DB for something simalar to expr
         * @param expr Query to search for
         * @return List of similar entries
         */
        QSqlError findSimilar(QString expr, QVector<Person> &pList, QVector<Computer> &cList);

        /**
         * @brief closeService - close service and save information
         * @return true if successful, false otherwise
         */
        bool closeService() { return true; /* db.saveData(); */ }
        /**
         * @brief startService - start service; load information
         * @return true if successful, false otherwise
         */
        QSqlError startService() { return db.init(); }

        /**
         * @brief addPerson - Add person to DB
         * @param p Person to add
         * @return true if successful, false otherwise
         */
        bool addPerson(Person p) { return db2.addPerson(p); }
        QSqlError addComputer(Computer c) { return cRepo.addComputer(c); }
        /**
         * @brief getPerson - Get person from DB
         * @param index Index of person to fetch
         * @param p Person object to store information
         * @return true if successful, false otherwise
         */
        bool getPerson(unsigned int index, Person &p) { return !db2.getPerson(index, p).isValid(); }
        QSqlError getComputer(Computer &c) { return cRepo.getComputer(c); }
        /**
         * @brief editPerson - Edit person in DB
         * @param index Index of person to edit
         * @param newInfo Person object containing new information
         * @return
         */
        QSqlError editPerson(Person newInfo)     { return db2.editPerson(newInfo); }
        QSqlError editComputer(Computer newInfo) { return cRepo.editComputer(newInfo); }

        /**
         * @brief loadDataFromFile - Load file into DB
         * @param fname File to load into DB
         * @return true if successful, false otherwise
         */
        bool loadDataFromFile(QString fname) { return true; /* db.importFromFile(fname); */ }

        /**
         * @brief setSort - Set how sortOrder for sorting
         * @param s Sort order for list of person objects
         */
        void setSort(utils::SORTS s) { cRepo.setSort(s); db2.setSort(s); }

    private:
        /**
         * @brief db - Database connection
         */
        DB db;
        PersonRepository db2;
        ComputerRepository cRepo;

        QVector<Person> moveAliveToBack (QVector<Person> pList);
        QVector<Person> moveAliveToFront(QVector<Person> pList);
};

#endif // PERSONSERVICE_H
