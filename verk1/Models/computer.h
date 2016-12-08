#ifndef COMPUTER_H
#define COMPUTER_H

#include <QString>
#include <QTextStream>

class Computer
{
    public:
        Computer() { }
        Computer(QString n, QString t, uint yb = -1, bool b = true, uint i = -1)
            : name(n), type(t), yearBuilt(yb), built(b), id(i) { }

       QString getName()   const { return name; }
       QString getType()   const { return type; }
       uint getYearBuilt() const { return yearBuilt; }
       bool getBuilt()     const { return built; }
       uint getId()        const { return id; }

       void setName(QString n)    { name = n; }
       void setType(QString t)    { type = t; }
       void setYearBuilt(uint yb) { yearBuilt = yb; }
       void setBuilt(bool b)      { built = b; }
       void setId(uint i)         { id = i; }

        friend bool operator ==(const Computer &lhs, const Computer &rhs);
        friend QTextStream& operator <<(QTextStream &out, const Computer &c);
        friend QTextStream& operator >>(QTextStream &in, Computer &c);

    private:
        QString name;
        QString type;
        uint yearBuilt;
        bool built;
        uint id;
};

#endif // COMPUTER_H
