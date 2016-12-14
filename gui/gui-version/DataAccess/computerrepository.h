#ifndef COMPUTERREPOSITORY_H
#define COMPUTERREPOSITORY_H

#include <QtSql>

#include "Models/computer.h"
#include "Utilities/utils.h"

class ComputerRepository
{
    public:
        ComputerRepository() { setSort(utils::IDASC); }
        ComputerRepository(utils::SORTS s)
            : sortOrder(s) { }

        QSqlError addComputer(const Computer c);
        QSqlError getComputer(Computer &c);
        QSqlError editComputer(const Computer newInfo);

        QVector<Computer> getAll();

        void setSort(utils::SORTS s) { sortOrder = s; }

        QSqlError findSimilar(QString expr, QVector<Computer> &cList);
    private:
        QSqlDatabase db;
        utils::SORTS sortOrder;

        void getIndexes(QSqlQuery q, int &indexI, int &indexN,
                        int &indexT, int &indexY, int &indexB);

        string getOrderBy();
        QSqlError getConnectedPersons(uint id, QVector<uint> &connectedPersons);
};

#endif // COMPUTERREPOSITORY_H
