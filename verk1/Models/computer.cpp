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
    out << "        Year built: " << c.yearBuilt << endl;
    out << "Has it been built?: " << (c.built ? "Yes" : "No") << endl;

    return out;
}

QTextStream& operator >>(QTextStream &in, Computer &c) {
    return in;
}
