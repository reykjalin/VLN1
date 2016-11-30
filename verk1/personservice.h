#ifndef PERSONSERVICE_H
#define PERSONSERVICE_H

#include <QObject>
#include <QVector>

#include "person.h"

using namespace std;

class PersonService : public QObject
{
        Q_OBJECT
    public:
        explicit PersonService(QObject *parent = 0)
            : QObject(parent) { }

        QVector<Person> getPersonList() { return pList; }

    private:
        QVector<Person> pList;
};

#endif // PERSONSERVICE_H
