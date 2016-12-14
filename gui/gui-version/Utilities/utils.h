#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "Models/person.h"

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
        static QString uitos(unsigned int n) { return QString::number(n); }

        // These functions are comparators used by stable_sort to sort
        // the person list
        static bool sortName(const Person &lhs, const Person &rhs);
        static bool sortGender(const Person &lhs, const Person &rhs);
        static bool sortBirth(const Person &lhs, const Person &rhs);
        static bool sortDeath(const Person &lhs, const Person &rhs);

        // ENUM for sort orders
        enum SORTS {
            IDASC,
            IDDESC,

            NAMEDESC,
            NAMEASC,

            GENDER_TYPE_DESC,
            GENDER_TYPE_ASC,

            BIRTH_BUILDYEAR_DESC,
            BIRTH_BUILDYEAR_ASC,

            DEATH_BUILT_DESC,
            DEATH_BUILT_ASC,

            FIRSTSORT = IDASC,
            LASTSORT = DEATH_BUILT_ASC
        };

        static bool isValidSort(SORTS s) { return s >= FIRSTSORT && s <= LASTSORT; }

        static bool isValidYear(uint year) { return year <= getCurrentYear(); }
        static uint getCurrentYear() { return QDate::currentDate().year(); }
};

#endif // UTILS_H
