#ifndef PERSONPRESENTATION_H
#define PERSONPRESENTATION_H

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QVector>

#include "personservice.h"
#include "person.h"
#include "utils.h"

using namespace std;

class PersonPresentation : public QObject
{
        Q_OBJECT
    public:
        // Initialize qout and qin to stdin and stdout
        // This to enable the use of QStrings
        explicit PersonPresentation(QObject *parent = 0);

    public slots:
        /**
         * @brief startPresentation - Start program. Contains main program loop
         */
        void startPresentation();

    signals:
        /**
         * @brief finished - signal emitted to tell the program to gracefully shut down
         */
        void finished();

    private:
        /**
         * @brief qout - Program output stream
         */
        QTextStream   qout;
        /**
         * @brief qin - Program input stream
         */
        QTextStream   qin;
        /**
         * @brief service - Person service object. Contains functions to interact with
         * the data layer
         */
        PersonService service;
        /**
         * @brief selectionDescriptions - Descriptions of main menu selections
         */
        QVector<QString> selectionDescriptions;

        // Various print functions. Mostly full of qout (print) commands.
        // Notably contain a lot of helper functions to simplify some of
        // the more complicated prints, like printing out the table of
        // person information stored
        void printMenu();

        // Functions to print the person list
        void printPersonList(QVector<Person> pList);
        void printListHeader(int longestN, int longestG, int longestId);
        void printDashes(int n);
        void printSpacing(int n);
        void printSeperator(int longestN, int longestG, int longestId);
        /**
         * @brief findLongestNameAndGender - helper function for printing person table
         * Allows easy access to correct spacing for the table
         * @param longestN - current longest name known
         * @param longestG - current longest gender known
         * @param pList - List of current persons
         */
        void findLongestNameAndGender(int &longestN, int &longestG, QVector<Person> pList);


        /**
         * @brief printSortMenu - selection menu for sort order
         */
        void printSortMenu();
        /**
         * @brief sort - allows user to select sort order
         */
        void sort();

        /**
         * @brief loadInfoFromFile - Contacts service layer with information on which
         * file to use to load information into the DB
         */
        void loadInfoFromFile();

        /**
         * @brief createPerson - Create empty person object
         * @return Empty person object
         */
        Person          createPerson();
        /**
         * @brief editPerson - allow user to edit information of some individual
         */
        void editPerson();
        void printEditMenu(Person p);

        // Change person object information
        void editPerson(Person &p);
        void changeName(Person &p);
        void changeGender(Person &p);
        void changeBirthYear(Person &p);
        void changeDeathYear(Person &p);

        /**
         * @brief find - Allow the user to search in the DB
         * @return List of person objects found
         */
        QVector<Person> find();

        /**
         * @brief The SELECTIONS enum is used to assign numbers to different choices
         * in the main menu
         */
        enum SELECTIONS {
            GETLIST = 1,
            ADDPERSON,
            EDITPERSON,
            SEARCH,
            ORDER,
            LOADFILE,
            SIZE = LOADFILE + 1 // used to know the size of selectionDescription
        };
};

#endif // PERSONPRESENTATION_H
