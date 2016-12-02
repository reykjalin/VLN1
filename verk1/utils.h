#ifndef UTILS_H
#define UTILS_H

#include <QString>

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
};

#endif // UTILS_H
