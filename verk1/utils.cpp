#include "utils.h"

int utils::stoi(QString s, bool &success) {
    success = true;
    return s.toInt(&success);
}

bool utils::sortName(const Person &lhs, const Person &rhs) {
    return lhs.getName() < rhs.getName();
}

bool utils::sortGender(const Person &lhs, const Person &rhs) {
    return lhs.getGender() < rhs.getGender();
}

bool utils::sortBirth(const Person &lhs, const Person &rhs) {
    return lhs.getBirthYear() < rhs.getBirthYear();
}

bool utils::sortDeath(const Person &lhs, const Person &rhs) {
    return lhs.getDeathYear() < rhs.getDeathYear();
}
