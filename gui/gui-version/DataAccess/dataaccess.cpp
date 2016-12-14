#include "dataaccess.h"

bool DataAccess::saveData() {
    // Open file, return false if fails
    QFile data(fileName);
    if(!openWriteFile(data))
        return false;

    // Write data to file in CSV format seperated by ';'
    // Format is:
    // Name;Gender;Year of birth;Year of death
    QTextStream fout(&data);
    for(int i = 0; i < pList.length(); i++) {
        fout << pList[i].getName()      << ";"
             << pList[i].getGender()    << ";"
             << pList[i].getBirthYear() << ";"
             << pList[i].getDeathYear() << endl;
    }

    return true;
}

bool DataAccess::readData(QString fname) {
    // Open file, return false if fails
    if(fname.isEmpty())
        fname = fileName;
    QFile data(fname);

    // If file doesn't exist, create empty file
    if(!data.exists()){
        if(!openWriteFile(data))
            return false;
        data.close();
    }
    // Open file to read data
    if(!openReadFile(data))
        return false;

    // Build QVector with list of person data
    QTextStream fin(&data);
    while(!fin.atEnd()) {
        QString line = fin.readLine();
        QStringList columns = line.split(";");

        // If column length is not 4, the data isn't using correct format
        // Format is:
        // Name;Gender;Year of birth;Year of death
        if(columns.length() != 4)
            return false;

        // Convert year of birth and year of death to int
        bool birthSuccess = true;
        bool deathSuccess = true;
        int birthYear = utils::stoi(columns[2], birthSuccess);
        int deathYear = utils::stoi(columns[3], deathSuccess);
        // If conversion fails, year was not a number
        //    => incorrect format or something went wrong
        //  Solution: Skip this entry, move to next entry
        if(!(birthSuccess && deathSuccess))
            continue;

        // Add if not already in list
        Person p(columns[0], columns[1], birthYear, deathYear);
        addPerson(p);
    }
    data.close();
    return true;
}

bool DataAccess::openWriteFile(QFile &file) {
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return false;
    return true;
}

bool DataAccess::openReadFile(QFile &file) {
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    return true;
}

bool DataAccess::importFromFile(QString fname) {
    if(QFile::exists(fname))
        return readData(fname);
    return false;
}

bool DataAccess::addPerson(Person p) {
    if(pList.contains(p))
        return false;

    pList.append(p);
    return true;
}

bool DataAccess::editPerson(unsigned int index, Person newInfo) {
    // unsigned, no negative numbers
    // cast to unsigned because length() returns int
    if(index >= static_cast<unsigned int>(pList.length()) )
        return false;

    pList[index].setName     (newInfo.getName()     );
    pList[index].setGender   (newInfo.getGender()   );
    pList[index].setBirthYear(newInfo.getBirthYear());
    pList[index].setDeathYear(newInfo.getDeathYear());
    return true;
}

bool DataAccess::getPerson(unsigned int index, Person &p) {
    // unsigned, no negative numbers
    // cast to unsigned because length() returns int
    if(index >= static_cast<unsigned int>(pList.length()) )
        return false;

    p = pList[index];
    return true;
}

void DataAccess::sort() {
    // Make sure the list is ALWAYS sorted by name within each group
    switch(sortOrder) {
        case utils::NAMEASC:              sortName(); break;
        case utils::GENDER_TYPE_ASC:      sortName(); sortGender(); break;
        case utils::BIRTH_BUILDYEAR_ASC:  sortName(); sortBirth();  break;
        case utils::DEATH_BUILT_ASC:      sortName(); sortDeath();  moveAliveToBack(); break;
        default:
            break;
    }
    // moveAliveToBack() call for sorting by death year is necessary so that
    // persons that are still alive (where deathYear = -1) go to the correct
    // place in the list
}

void DataAccess::moveAliveToBack() {
    // Move every person with deathYear = -1 to the back of the list
    for(int i = 0; i < pList.length(); i++) {
        if(pList[0].getDeathYear() != -1)
            break;
        pList.append(pList[0]);
        pList.removeFirst();
    }
}
