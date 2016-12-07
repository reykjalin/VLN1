#include "personservice.h"

QVector<Person> PersonService::findSimilar(QString expr) {
    expr = expr.toLower();
    QVector<Person> found;

    QVector<Person> pList = db.getPersonList();
    for(int i = 0; i < pList.length(); i++) {
        // Check everythingi person has for something similar
        if(pList[i].getName().toLower().contains(expr)         ||
           pList[i].getGender().toLower().contains(expr)       ||
           utils::itos(pList[i].getBirthYear()).contains(expr) ||
           utils::itos(pList[i].getDeathYear()).contains(expr))
        {
            found.append(pList[i]);
        }
    }
    return found;
}


