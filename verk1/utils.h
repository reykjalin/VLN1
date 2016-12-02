#ifndef UTILS_H
#define UTILS_H

#include <QString>

#include "person.h"

class utils
{
    public:
        utils() { }

        /**
         * @brief stoi - Convert string to integer
         * @param s string to convert
         * @param success variable that tells you whether the conversion
         * was successful
         * @return integer in QString format
         */
        static int stoi(QString s, bool &success);
        /**
         * @brief itos - Convert integer to string
         * @param n integer to convert
         * @return string of the int passed in variable n
         */
        static QString itos(int n) { return QString::number(n); }

        static bool sortName(const Person &lhs, const Person &rhs);
        static bool sortGender(const Person &lhs, const Person &rhs);
        static bool sortBirth(const Person &lhs, const Person &rhs);
        static bool sortDeath(const Person &lhs, const Person &rhs);

        enum SORTS {
            NAME,
            GENDER,
            BIRTH,
            DEATH
        };

};

#endif // UTILS_H
