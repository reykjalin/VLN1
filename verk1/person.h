#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <iostream>

#include <QDate>
#include <QString>
#include <QTextStream>

using namespace std;

class Person
{
    public:
        Person();
        Person(QString n, QString g, int b = 0, int d = 0)
            :  name(n), gender(g), birthYear(b), deathYear(d) { }
        Person(const Person &copyMe)
            : name(copyMe.name), gender(copyMe.gender),
              birthYear(copyMe.birthYear), deathYear(copyMe.deathYear) { }

        QString getName() const   { return name; }
        QString getGender() const { return gender; }
        int getBirthYear() const { return birthYear; }
        int getDeathYear() const { return deathYear; }

        void setName(QString n)   { name = n; }
        void setGender(QString g) { gender = g; }
        void setBirthYear(int b) { birthYear = b; }
        void setDeathYear(int d) { deathYear = d; }

        bool operator >(const Person &rhs);
        friend QTextStream& operator <<(QTextStream &out, const Person &p);
        friend QTextStream& operator >>(QTextStream &in, Person &p);

    private:
        QString name;
        QString gender;
        int birthYear;
        int deathYear;
};

#endif // PERSON_H