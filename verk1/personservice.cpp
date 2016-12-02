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

void PersonService::sort() {
    // Make sure the list is ALWAYS sorted by name within each group
    switch(sortOrder) {
        case utils::NAME:   sortName(); break;
        case utils::GENDER: sortName(); sortGender(); break;
        case utils::BIRTH:  sortName(); sortBirth();  break;
        case utils::DEATH:  sortName(); sortDeath();  moveAliveToBack(); break;
        default:
            break;
    }
}

void PersonService::moveAliveToBack() {
    for(int i = 0; i < pList.length(); i++) {
        if(pList[0].getDeathYear() != -1)
            break;
        pList.append(pList[0]);
        pList.removeFirst();
    }
}
