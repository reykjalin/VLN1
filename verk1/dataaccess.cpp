#include "dataaccess.h"

DataAccess::DataAccess() {
    fileName = "personDB.csv";
}

bool DataAccess::saveData(QVector<Person> pList) {
    // Open file, return false if fails
    QFile data(fileName);
    if(!openWriteFile(data))
        return false;

    // Write data to file in CSV format seperated by ';'
    // Columns are: Name ; Gender ; Year of birth ; Year of death
    QTextStream fout(&data);
    for(int i = 0; i < pList.length(); i++) {
        fout << pList[i].getName()      << ";"
             << pList[i].getGender()    << ";"
             << pList[i].getBirthYear() << ";"
             << pList[i].getDeathYear() << endl;
    }

    return true;
}

bool DataAccess::readData(QVector<Person> &pList, QString fname) {
    // Open file, return false if fails
    if(fname.isEmpty())
        fname = fileName;
    QFile data(fname);
    if(!data.exists()){
        openWriteFile(data);
        data.close();
    }
    if(!openReadFile(data))
        return false;

    // Build QVector with list of person data
    QTextStream fin(&data);
    while(!fin.atEnd()) {
        QString line = fin.readLine();
        QStringList columns = line.split(";");

        // If column length is not 4, the data isn't using correct format
        // Format is: Name ; Gender ; Year of birth ; Year of death
        if(columns.length() != 4)
            return false;

        // Convert year of birth and year of death to int
        bool birthSuccess = true;
        bool deathSuccess = true;
        int birthYear = utils::stoi(columns[2], birthSuccess);
        int deathYear = utils::stoi(columns[3], deathSuccess);
        // If conversion fails, year was not a number
        //    => incorrect format or something went wrong
        if(!(birthSuccess && deathSuccess))
            return false;

        // Add if not already in list
        Person p(columns[0], columns[1], birthYear, deathYear);
        if(!pList.contains(p))
            pList.append(p);
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

bool DataAccess::importFromFile(QVector<Person> &pList, QString fname) {
    if(QFile::exists(fname))
        return readData(pList, fname);
    return false;
}
