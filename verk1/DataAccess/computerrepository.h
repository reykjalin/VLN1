#ifndef COMPUTERREPOSITORY_H
#define COMPUTERREPOSITORY_H

#include <QtSql>

#include "Models/computer.h"
#include "Utilities/utils.h"

class ComputerRepository
{
    public:
        ComputerRepository() { }

        QSqlError initRepo();

        QSqlError addComputer(const Computer c);
        QSqlError getComputer(uint id, Computer &c);
        QSqlError editComputer(const Computer newInfo);

        QVector<Computer> getAll();
    private:
        QSqlDatabase db;

        void getIndexes(QSqlQuery q, int &indexI, int &indexN,
                        int &indexT, int &indexY, int &indexB);
};

#endif // COMPUTERREPOSITORY_H
