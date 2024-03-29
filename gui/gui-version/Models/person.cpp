#include "person.h"

bool isValidYear(int year);
int getCurrentYear();
bool isValidDeathYear(int death, int birth);

Person::Person() {
    name = "";
    gender = "";
    birthYear = -1;
    deathYear = -1;
    id = -1;
}

QTextStream& operator <<(QTextStream &out, const Person &p) {
    out << "         Name: " << p.name << endl;
    out << "       Gender: " << p.gender << endl;
    out << "Year of birth: " << p.birthYear << endl;
    out << "Year of death: ";

    if(p.deathYear == -1)
        out << "still alive" << endl;
    else
        out << p.deathYear << endl;
    return out;
}

QTextStream& operator >>(QTextStream &in, Person &p) {
    cout << "Enter name: "; p.name = in.readLine();
    cout << "Enter gender [male/female/other]: "; p.gender = in.readLine();

    do {
        cout << "Enter year of birth [0 - " << getCurrentYear() << "]: "; in >> p.birthYear;
        if(!Person::isValidYear(p.birthYear))
            cout << "Invalid year of birth." << endl;
    } while(!Person::isValidYear(p.birthYear));

    do {
        cout << "Enter year of death [-1 if still alive]: "; in >> p.deathYear;
        if(!Person::isValidDeathYear(p.deathYear, p.birthYear))
            cout << "Invalid year of death." << endl;
    } while(!Person::isValidDeathYear(p.deathYear, p.birthYear));

    return in;
}

bool Person::isValidYear(int year) {
    return year >= 0 && year <= getCurrentYear();
}
bool Person::isValidDeathYear(int death, int birth) {
    return death == -1 || (death >= birth && isValidYear(death));
}

int getCurrentYear() {
    return QDate::currentDate().year();
}

bool operator ==(const Person &lhs, const Person &rhs) {
    return  lhs.name      == rhs.name      &&
            lhs.gender    == rhs.gender    &&
            lhs.birthYear == rhs.birthYear &&
            lhs.deathYear == rhs.deathYear;
}
