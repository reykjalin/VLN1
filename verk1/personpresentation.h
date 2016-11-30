#ifndef PERSONPRESENTATION_H
#define PERSONPRESENTATION_H

#include <QObject>
#include <QString>
#include <QTextStream>

#include <iostream>
#include "personservice.h"
#include "person.h"

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
        PersonService service;
        QTextStream qout;
        QTextStream qin;
};

#endif // PERSONPRESENTATION_H
