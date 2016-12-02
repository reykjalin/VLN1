#include "personservice.h"

bool PersonService::addPerson(Person p) {
    if(!pList.contains(p)) {
        pList.append(p);
        return true;
    }
    return false;
}

QVector<Person> PersonService::findSimilar(QString expr) {
    expr = expr.toLower();
    QVector<Person> found;
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
