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

        bool readDataFromDB(QVector<Person> &pList) { return readData(pList, fileName); }
        bool importFromFile(QVector<Person> &pList, QString fname);
        bool saveData(QVector<Person> pList);
    private:
        QString fileName;

        bool readData(QVector<Person> &pList, QString fname);
        bool openWriteFile(QFile &file);
        bool openReadFile(QFile &file);
};

#endif // DATAACCESS_H
