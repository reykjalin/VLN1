#include "utils.h"

int utils::stoi(QString s, bool &success) {
    success = true;
    return s.toInt(&success);
}
