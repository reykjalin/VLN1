#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>

#include "person.h"
#include "utils.h"

class DataAccess
{
    public:
        DataAccess();
        DataAccess(QString fn)
            : fileName(fn) { }

        bool readData(QVector<Person> &pList);
        bool saveData(QVector<Person> pList);
    private:
        QString fileName;

        bool openWriteFile(QFile &file);
        bool openReadFile(QFile &file);
};

#endif // DATAACCESS_H
