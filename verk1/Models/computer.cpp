#include "computer.h"


bool operator ==(const Computer &lhs, const Computer &rhs) {
    return  lhs.name      == rhs.name      &&
            lhs.type      == rhs.type      &&
            lhs.yearBuilt == rhs.yearBuilt &&
            lhs.built     == rhs.built     &&
            lhs.id        == rhs.id;
}

QTextStream& operator <<(QTextStream &out, const Computer &c) {
    out << "              Name: " << c.name << endl;
    out << "              Type: " << c.type << endl;
    out << "Has it been built?: " << (c.built ? "Yes" : "No") << endl;
    if(c.built)
        out << "        Year built: " << c.yearBuilt << endl;

    return out;
}

QTextStream& operator >>(QTextStream &in, Computer &c) {
    cout << "Enter name: "; c.name = in.readLine();
    cout << "Enter type: "; c.type = in.readLine();
    QString built;

    do {
        cout << "Was the computer built? [y/n]: ";
        in >> built;
        if(built.toLower() != "y" && built.toLower() != "n")
            cout << "Illegal input, try again" << endl;
    } while(built.toLower() != "y" && built.toLower() != "n");

    if(built.toLower() == "y") {
        c.built = true;
        do {
            cout << "Year built: " ;
            in >> c.yearBuilt;
            if(!utils::isValidYear(c.yearBuilt))
                cout << "Invalid year, try again" << endl;
        } while(!utils::isValidYear(c.yearBuilt));
    }
    else {
        c.built     = false;
        c.yearBuilt = -1;
    }

    return in;
}
