#include "personservice.h"

QSqlError PersonService::findSimilar(QString expr, QVector<Person> &pList, QVector<Computer> &cList) {
    QSqlError e = db2.findSimilar(expr, pList);
    if(e.isValid())
        return e;
    e = cRepo.findSimilar(expr, cList);
    return e;
}



QVector<Person> PersonService::moveAliveToBack(QVector<Person> pList) {
    // Move every person with deathYear = -1 to the back of the list
    for(int i = 0; i < pList.length(); i++) {
        if(pList[0].getDeathYear() != -1)
            break;
        pList.append(pList[0]);
        pList.removeFirst();
    }
    return pList;
}

QVector<Person> PersonService::moveAliveToFront(QVector<Person> pList) {
    // Move every person with deathYear = -1 to the front of the list
    for(int i = 0; i < pList.length(); i--) {
        if(pList.last().getDeathYear() != -1)
            break;
        pList.prepend(pList.last());
        pList.removeLast();
    }
    return pList;
}

QVector<Person> PersonService::getPersonList() {
    if(db2.getSortOrder() == utils::DEATH_BUILT_ASC)  return moveAliveToBack(db2.getAll());
    if(db2.getSortOrder() == utils::DEATH_BUILT_DESC) return moveAliveToFront(db2.getAll());
    else                                              return db2.getAll();
}
