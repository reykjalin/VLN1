#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>

#include "Models/person.h"
#include "Utilities/utils.h"

class DataAccess
{
    public:
        DataAccess() { fileName = "personDB.csv"; sortOrder = utils::NAME; }
        DataAccess(QString fn)
            : fileName(fn) { }

        /**
         * @brief readDataFromDB - read data from database (csv file for now)
         * @param pList list to store info
         * @return true if successful, false otherwise
         */
        bool readDataFromDB() { return readData(fileName); }

        /**
         * @brief importFromFile - import data from csv file
         * Make sure that the file is on the form:
         * Name;Gender;Birthyear;Deathyear
         * Name;Gender;Birthyear;Deathyear
         * ...
         * @param pList - list to store info
         * @param fname - name of file to import from
         * @return true if successful, false otherwise
         */
        bool importFromFile(QString fname);

        /**
         * @brief saveData - Save current session
         * @param pList - Data to save
         * @return true if successful, false otherwise
         */
        bool saveData();

        /**
         * @brief addPerson - Add person to DB
         * @param p Person to add
         * @return true if successful, false otherwise e.g. if person is already in DB
         */
        bool addPerson(Person p);
        /**
         * @brief editPerson - Edit person information in the DB
         * @param index Index to this person in list (shown in table of persons)
         * @return true if successful, false otherwise e.g. if person doesn't exist
         */
        bool editPerson(unsigned int index, Person newInfo);
        /**
         * @brief getPerson - Get information on individual in DB
         * @param index Index to the requested person
         * @param p Person object to store the person information
         * @return true if successful, false otherwise
         */
        bool getPerson(unsigned int index, Person &p);
        /**
         * @brief getPersonList - get list of Persons in database
         * @return List containing person information
         */
        QVector<Person> getPersonList() { return pList; }

        /**
         * @brief sort - sort list of person objects
         */
        void sort();
        /**
         * @brief setSort - Set how sortOrder for sorting
         * @param s Sort order for list of person objects
         */
        void setSort(utils::SORTS s) { sortOrder = s; }

    private:
        /**
         * @brief fileName - Name of the DB file (currently .csv file)
         */
        QString fileName;
        /**
         * @brief pList - list of Person objects
         */
        QVector<Person> pList;
        /**
         * @brief sortOrder - Current sort order
         */
        utils::SORTS sortOrder;

        /**
         * @brief readData - read data from csv file
         * @param pList - list to store the data
         * @param fname - name of the file to read from
         * @return true if successful, false otherwise
         */
        bool readData(QString fname);
        /**
         * @brief openWriteFile - Open file to write data
         * @param file File to open
         * @return true if successful, false otherwise
         */
        bool openWriteFile(QFile &file);
        /**
         * @brief openReadFile - Open file to read data
         * @param file File to open
         * @return true if successful, false otherwise
         */
        bool openReadFile(QFile &file);

        void sortName()   { stable_sort(pList.begin(), pList.end(), utils::sortName); }
        void sortGender() { stable_sort(pList.begin(), pList.end(), utils::sortGender); }
        void sortBirth()  { stable_sort(pList.begin(), pList.end(), utils::sortBirth); }
        void sortDeath()  { stable_sort(pList.begin(), pList.end(), utils::sortDeath); }

        /**
         * @brief moveAliveToBack - Moves alive persons to the back of the list.
         * Since deathYear has value -1 if still alive, this is necessary so that
         * people that are still alive appear at the correct point in a sorted list.
         */
        void moveAliveToBack();
};

#endif // DATAACCESS_H
