#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QTextStream>
#include <QVector>

#include <iostream>
using namespace std;

class Person
{
    public:
        Person();
        Person(QString n, QString g, int b = 0, int d = 0, QVector<uint> c = QVector<uint>(), uint i = -1)
            : name(n), gender(g), birthYear(b), deathYear(d), connectedComputers(c), id(i) { }
        Person(const Person &copyMe)
            : name(copyMe.name), gender(copyMe.gender),
              birthYear(copyMe.birthYear), deathYear(copyMe.deathYear),
              connectedComputers(copyMe.connectedComputers), id(copyMe.id) { }

        QString getName()        const { return name; }
        QString getGender()      const { return gender; }
        int getBirthYear()       const { return birthYear; }
        int getDeathYear()       const { return deathYear; }
        uint getId()             const { return id; }
        QVector<uint> getConns() const { return connectedComputers; }

        void setName(QString n)        { name = n; }
        void setGender(QString g)      { gender = g; }
        void setBirthYear(int b)       { birthYear = b; }
        void setDeathYear(int d)       { deathYear = d; }
        void setId(uint i)             { id = i; }
        void setConns(QVector<uint> c) { connectedComputers = c; }

        friend bool operator ==(const Person &lhs, const Person &rhs);
        friend QTextStream& operator <<(QTextStream &out, const Person &p);
        friend QTextStream& operator >>(QTextStream &in, Person &p);

        static bool isValidYear(int year);
        static bool isValidDeathYear(int death, int birth);

    private:
        QString name;
        QString gender;
        int birthYear;
        int deathYear;
        QVector<uint> connectedComputers;
        uint id;
};

#endif // PERSON_H
