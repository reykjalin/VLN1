#ifndef COMPUTER_H
#define COMPUTER_H

#include <QString>
#include <QTextStream>
#include <QVector>

#include <iostream>
#include "Utilities/utils.h"
using namespace std;

class Computer
{
    public:
        Computer() { }
        Computer(QString n, QString t, uint yb = -1, bool b = true,
                 QVector<uint> c = QVector<uint>(), uint i = -1)
            : name(n), type(t), yearBuilt(yb), built(b), connectedPersons(c), id(i) { }
        Computer(const Computer &copyMe)
            : name(copyMe.name), type(copyMe.type), yearBuilt(copyMe.yearBuilt),
              built(copyMe.built), connectedPersons(copyMe.connectedPersons), id(copyMe.id) { }

       QString getName()        const { return name; }
       QString getType()        const { return type; }
       uint getYearBuilt()      const { return yearBuilt; }
       bool getBuilt()          const { return built; }
       uint getId()             const { return id; }
       QVector<uint> getConns() const { return connectedPersons; }

       void setName(QString n)        { name = n; }
       void setType(QString t)        { type = t; }
       void setYearBuilt(uint yb)     { yearBuilt = yb; }
       void setBuilt(bool b)          { built = b; }
       void setId(uint i)             { id = i; }
       void setConns(QVector<uint> c) { connectedPersons = c; }

       friend bool operator ==(const Computer &lhs, const Computer &rhs);
       friend QTextStream& operator <<(QTextStream &out, const Computer &c);
       friend QTextStream& operator >>(QTextStream &in, Computer &c);

    private:
        QString name;
        QString type;
        uint yearBuilt;
        bool built;
        QVector<uint> connectedPersons;
        uint id;
};

#endif // COMPUTER_H
