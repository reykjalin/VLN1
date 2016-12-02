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
        explicit PersonPresentation(QObject *parent = 0)
            : QObject(parent), qout(stdout), qin(stdin) { }

    public slots:
        void startPresentation();

    signals:
        void finished();

    private:
        QTextStream   qout;
        QTextStream   qin;
        PersonService service;

        void findLongestNameAndGender(int &longestN, int &longestG, QVector<Person> pList);
        void printMenu();
        void printPersonList(QVector<Person> pList);
        void printListHeader(int longestN, int longestG, int longestId);
        void printDashes(int n);
        void printSpacing(int n);
        void printSeperator(int longestN, int longestG, int longestId);
        void printSortMenu();
        void sort();

        Person          createPerson();
        QVector<Person> find();

        enum SELECTIONS {
            GETLIST = 1,
            ADDPERSON,
            SEARCH,
            ORDER
        };
};

#endif // PERSONPRESENTATION_H
