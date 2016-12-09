#include "personpresentation.h"

PersonPresentation::PersonPresentation(QObject *parent)
    : QObject(parent), qout(stdout), qin(stdin)
{
    // Initialize selection descriptions for main menu
    selectionDescriptions = QVector<QString>(SIZE);
    selectionDescriptions.insert(
                GETLIST, "Get a list of known individuals in the history of Computer Science");
    selectionDescriptions.insert(
                ADDPERSON, "Add new person");
    selectionDescriptions.insert(
                EDITPERSON, "Edit information on individual in the list");
    selectionDescriptions.insert(
                GETCOMPUTERLIST, "Get a list of computers");
    selectionDescriptions.insert(
                ADDCOMPUTER, "Add new computer");
    selectionDescriptions.insert(
                EDITCOMPUTER, "Edit information on computer in the list");
    selectionDescriptions.insert(
                SEARCH, "Search for famous individuals from the history of Computer Science");
    selectionDescriptions.insert(
                ORDER, "Choose the order in which the list of individuals will appear");
    selectionDescriptions.insert(
                GET_PERSON_INFO, "Get detailed information about famous individiual");
    selectionDescriptions.insert(
                GET_COMP_INFO, "Get detailed information about a specific computer");
    selectionDescriptions.insert(
                LOADFILE, "Import data from external file");
}

void PersonPresentation::startPresentation() {
    // Skip loop and don't save data if isOK becomes false
    bool isOK = true;
    if(service.startService().isValid()) {
        qout << "Something went wrong when accessing saved data." << endl;
        isOK = false;
    }

    while(isOK) {
        printMenu();

        QString input;
        qin >> input;
        if(input == "q")
            break;

        if(input == utils::itos(GETLIST)) {

            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(ADDPERSON)) {

            qout << endl;
            qin.read(1); // Remove extra newlines in buffer
            if(!service.addPerson(createPerson()))
                qout << "This person is already registered in the database."
                     << endl;

        }
        else if(input == utils::itos(SEARCH)) {

            QVector<Person> pList;
            QVector<Computer> cList;
            find(pList, cList);
            if(!pList.length())
                qout << "No individuals found." << endl << endl;
            else {
                qout << "Individuals found:";
                printPersonList(pList);
            }

            if(!cList.length())
                qout << "No computers found." << endl;
            else {
                qout << "Computers found:";
                printComputerList(cList);
            }

        }
        else if(input == utils::itos(ORDER)) {

            sort();         // Configure sort
            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(LOADFILE)) {

            loadInfoFromFile();
            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(EDITPERSON)) {

            editPerson();
            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(GETCOMPUTERLIST)) {

            QVector<Computer> cList = service.getComputerList();
            printComputerList(cList);

        }
        else if(input == utils::itos(ADDCOMPUTER)) {

            qout << endl;
            qin.read(1); // Remove extra newlines in buffer
            QSqlError e = service.addComputer(createComputer());
            if(e.isValid())
                qout << "Something went wrong when registering new computer." << endl
                     << e.text();
        }
        else if(input == utils::itos(EDITCOMPUTER)) {

            editComputer();
            printComputerList(service.getComputerList());

        }
        else if(input == utils::itos(GET_PERSON_INFO)) {
            uint id = selectPerson("details on");
            Person toGet;
            toGet.setId(id);
            service.getPerson(id, toGet);
            printPersonDetails(toGet);
        }
        else if(input == utils::itos(GET_COMP_INFO)) {
            uint id = selectComputer("details on");
            Computer toGet;
            toGet.setId(id);
            service.getComputer(toGet);
            printComputerDetails(toGet);
        }
        else
            qout << "Invalid input." << endl;
    }

    if(isOK)
        qout << "Saving new data..." << endl;

    // Don't save data if something went wrong when accessing saved data
    if(isOK && !service.closeService())
        qout << "Something went wrong when saving new data to database. "
             << "Please check data integrity" << endl;

    // Close program
    qout << "Closing." << endl;
    emit finished();
}

void PersonPresentation::printMenu() {
    qout << endl;
    qout << "What do you want to do?" << endl;

    for(int i = GETLIST; i < SIZE; i++)
        qout << "[" + utils::itos(i) + "] " + selectionDescriptions[i] << endl;

    qout << "[q] Quit" << endl << endl;
    qout << "Selection: ";
    qout.flush();
}

void PersonPresentation::printPersonList(QVector<Person> pList) {
    qout << endl;
    if(!pList.length()) {
        qout << "No data" << endl << endl;
        return;
    }

    int longestName   = 4;  // length of string "Name"
    int longestGender = 5;  // length of string "other"
    int yearOfBirth   = 13; // length of string "year of birth"
    // get longest id
    int longestId = utils::itos(pList.last().getId()).length();
    longestId = (longestId < 2 ? 2 : longestId);

    findLongestNameAndGender(longestName, longestGender, pList);

    printSeperator(longestName, longestGender, longestId);
    printListHeader(longestName, longestGender, longestId);
    int i;
    for(i = 0; i < pList.length(); i++) {
        qout << "|";
        printSpacing(longestId - utils::uitos(pList[i].getId()).length());
        qout << " " + utils::uitos(pList[i].getId()) + " ";

        qout << "| " << pList[i].getName() << " ";
        printSpacing(longestName - pList[i].getName().length());

        qout << "| " << pList[i].getGender() << " ";
        printSpacing(longestGender - pList[i].getGender().length());

        qout << "|";
        printSpacing(yearOfBirth - utils::itos(pList[i].getBirthYear()).length());
        qout << " " << utils::itos(pList[i].getBirthYear()) << " ";

        QString d = utils::itos(pList[i].getDeathYear());
        if(d == "-1")
            d = "still alive";
        qout << "|";
        printSpacing(yearOfBirth - d.length());
        qout << " " << d << " ";
        qout << "|" << endl;

        if(i != 0 && !(i % 4))
            printSeperator(longestName, longestGender, longestId);
    }
    if((i % 4) != 1 || i == 1)
        printSeperator(longestName, longestGender, longestId);
    qout << endl;
}

void PersonPresentation::printComputerList(QVector<Computer> cList) {
    qout << endl;
    if(!cList.length()) {
        qout << "No data" << endl << endl;
        return;
    }

    int longestName  = 4;
    int longestType  = 4;
    int yearBuilt    = 10;
    int longestBuilt = 6;
    // get longest id
    int longestId = utils::uitos(cList.last().getId()).length();
    longestId = (longestId < 2 ? 2 : longestId);

    findLongestNameAndType(longestName, longestType, cList);

    printComputerListSeperator(longestName, longestType, longestId);
    printComputerListHeader(longestName, longestType, longestId);
    int i;
    for(i = 0; i < cList.length(); i++) {
        qout << "|";
        printSpacing(longestId - utils::uitos(cList[i].getId()).length());
        qout << " " + utils::uitos(cList[i].getId()) + " ";

        qout << "| " << cList[i].getName() << " ";
        printSpacing(longestName - cList[i].getName().length());

        qout << "| " << cList[i].getType() << " ";
        printSpacing(longestType - cList[i].getType().length());

        qout << "| ";
        if(cList[i].getBuilt()) {
            qout << "Yes";
            printSpacing(longestBuilt - 3);
        }
        else {
            qout << "No";
            printSpacing(longestBuilt - 2);
        }

        qout << " |";
        if(cList[i].getBuilt()) {
            printSpacing(yearBuilt - utils::itos(cList[i].getYearBuilt()).length());
            qout << " " << utils::uitos(cList[i].getYearBuilt()) << " ";
        }
        else {
            printSpacing(yearBuilt - 3);
            qout << " N/A ";
        }

        qout << "|" << endl;
    }
    if((i % 4) != 1 || i == 1)
        printComputerListSeperator(longestName, longestType, longestId);
    qout << endl;
}

void PersonPresentation::printListHeader(int longestN, int longestG, int longestId) {
    qout << "| ID ";
    printSpacing(longestId - 2);
    qout << "| Name ";
    printSpacing(longestN - 4);
    qout << "| Gender ";
    printSpacing(longestG - 6);
    qout << "| Year of birth | Year of death |" << endl;
    printSeperator(longestN, longestG, longestId);
}

void PersonPresentation::printComputerListHeader(int longestN, int longestT, int longestId) {
    qout << "| ID ";
    printSpacing(longestId - 2);
    qout << "| Name ";
    printSpacing(longestN - 4);
    qout << "| Type ";
    printSpacing(longestT - 4);
    qout << "| Built? | Year Built |" << endl;
    printComputerListSeperator(longestN, longestT, longestId);
}

void PersonPresentation::printComputerListSeperator(int longestN, int longestT, int longestId) {
    qout << "|";
    printDashes(longestId + 2);
    qout << "+";
    printDashes(longestN + 2);
    qout << "+";
    printDashes(longestT + 2);
    qout << "+";
    printDashes(8);
    qout << "+";
    printDashes(12);
    qout << "|" << endl;
}

void PersonPresentation::printSeperator(int longestN, int longestG, int longestId) {
    qout << "|";
    printDashes(longestId + 2);
    qout << "+";
    printDashes(longestN + 2);
    qout << "+";
    printDashes(longestG + 2);
    qout << "+";
    printDashes(15);
    qout << "+";
    printDashes(15);
    qout << "|" << endl;
}

void PersonPresentation::printDashes(int n) {
    for(int i = 0; i < n; i++)
        qout << "-";
}

void PersonPresentation::printSpacing(int n) {
    for(int i = 0; i < n; i++)
        qout << " ";
}

Person PersonPresentation::createPerson() {
    Person p;
    qin >> p;
    return p;
}

Computer PersonPresentation::createComputer() {
    Computer c;
    qin >> c;
    return c;
}

void PersonPresentation::findLongestNameAndType(int &longestN, int &longestT,
                                                QVector<Computer> cList)
{
    for(int i = 0; i < cList.length(); i++) {
        if(cList[i].getName().length() > longestN)
            longestN = cList[i].getName().length();
        if(cList[i].getType().length() > longestT)
            longestT = cList[i].getType().length();
    }
}

void PersonPresentation::findLongestNameAndGender(int &longestN, int &longestG,
                                                 QVector<Person> pList)
{
    for(int i = 0; i < pList.length(); i++) {
        if(pList[i].getName().length() > longestN)
            longestN = pList[i].getName().length();
        if(pList[i].getGender().length() > longestG)
            longestG = pList[i].getGender().length();
    }
}

void PersonPresentation::find(QVector<Person> &pList, QVector<Computer> &cList) {
    qout << endl;
    QString expression;
    qout << "Search for: ";
    qout.flush();

    qin.read(1);            // Clear input buffer
    expression = qin.readLine();

    if(expression.toLower().contains("alive") ||
       expression.toLower().contains("still"))
    {
        QSqlError e = service.findSimilar(expression, pList, cList);
        QSqlError e2 = service.findSimilar("-1", pList, cList);
        if(e.isValid() || e2.isValid())
            qout << "Something went wrong when searching" << endl
                 << e.text() << endl
                 << e2.text() << endl;
    }
    else if(expression.toLower().contains("no")) {
        // 0 from database is changed to "No" in table representation
        QSqlError e = service.findSimilar(expression, pList, cList);
        QSqlError e2 = service.findSimilar("0", pList, cList);
        if(e.isValid() || e2.isValid())
            qout << "Something went wrong when searching" << endl
                 << e.text() << endl
                 << e2.text() << endl;
    }
    else if(expression.toLower().contains("yes")) {
        // 1 from database is changed to "Yes" in table representation
        QSqlError e = service.findSimilar(expression, pList, cList);
        QSqlError e2 = service.findSimilar("1", pList, cList);
        if(e.isValid() || e2.isValid())
            qout << "Something went wrong when searching" << endl
                 << e.text() << endl
                 << e2.text() << endl;
    }
    else {
        QSqlError e = service.findSimilar(expression, pList, cList);
        if(e.isValid())
            qout << "Something went wrong when searching" << endl
                 << e.text() << endl;
    }
}

void PersonPresentation::sort() {
    QString sort;

    bool success = false;
    utils::SORTS s;
    do {
        printSortMenu();
        qin >> sort;

        s = static_cast<utils::SORTS>(utils::stoi(sort, success));

        if(!utils::isValidSort(s) || !success)
            qout << "Invalid choice, try again" << endl;

    } while(!utils::isValidSort(s) || !success);

    service.setSort(s);
    qout << endl;
}

void PersonPresentation::printSortMenu() {
    qout << endl;
    qout << "[" + utils::itos(utils::IDASC)                + "] ID ascending"      << endl;
    qout << "[" + utils::itos(utils::IDDESC)               + "] ID descending"     << endl;
    qout << "[" + utils::itos(utils::NAMEDESC)             + "] Name descending"   << endl;
    qout << "[" + utils::itos(utils::NAMEASC)              + "] Name ascending"    << endl;

    qout << "[" + utils::itos(utils::GENDER_TYPE_DESC)     +
            "] Gender (individuals) / Type (computers) descending" << endl;
    qout << "[" + utils::itos(utils::GENDER_TYPE_ASC)      +
            "] Gender (individuals) / Type (computers) ascending"  << endl;
    qout << "[" + utils::itos(utils::BIRTH_BUILDYEAR_DESC) +
            "] Year of birth (individuals) / Year built (computers) descending"    << endl;
    qout << "[" + utils::itos(utils::BIRTH_BUILDYEAR_ASC)  +
            "] Year of birth (individuals) / Year built (computers) ascending"     << endl;
    qout << "[" + utils::itos(utils::DEATH_BUILT_DESC)     +
            "] Year of death (individuals) / Was it built? (computers) descending" << endl;
    qout << "[" + utils::itos(utils::DEATH_BUILT_ASC)      +
            "] Year of death (individuals) / Was it built? (computers) ascending"  << endl;

    qout << "Any other selection will bring you back to the main menu" << endl << endl;
    qout << "Sort by: ";
    qout.flush();
}

void PersonPresentation::editComputer() {
    int id = selectComputer("to edit");

    Computer toEdit;
    toEdit.setId(id);

    qout << toEdit.getId() << endl;
    if(service.getComputer(toEdit).isValid()) {
        qout << "Selected ID doesn't exist" << endl;
        return;
    }

    editComputer(toEdit);

    QSqlError e = service.editComputer(toEdit);
    if(e.isValid()) {
        qout << "An error occurred when updating DB" << endl;
        return;
    }
}

void PersonPresentation::editPerson() {
    int id = selectPerson("to edit");

    Person toEdit;
    // Get person info
    if(service.getPerson(id, toEdit)) {
        // Edit info
        editPerson(toEdit);
        // Edit info in DB
        QSqlError e = service.editPerson(toEdit);
        if(e.isValid())
            qout << "An error occured while editing information:" << endl
                 << e.text() << endl;
    }
    qout << "Selected ID doesn't exist" << endl;
}

void PersonPresentation::printEditComputerMenu(Computer c) {
    qout << endl;
    qout << "Editing: " << endl;
    qout << c << endl;

    qout << "Which do you want to change?"    << endl;
    qout << "1. Name"                         << endl;
    qout << "2. Type"                         << endl;
    qout << "3. Year built"                   << endl;
    qout << "4. Add connection to individual" << endl;
    qout << "5. Done, return to main menu"    << endl << endl;
}

void PersonPresentation::printEditMenu(Person p) {
    qout << endl;
    qout << "Editing: " << endl;
    qout << p << endl;

    qout << "Which do you want to change?"    << endl;
    qout << "1. Name"                         << endl;
    qout << "2. Gender"                       << endl;
    qout << "3. Year of birth"                << endl;
    qout << "4. Year of death"                << endl;
    qout << "5. Add connection to a computer" << endl;
    qout << "6. Done, return to main menu"    << endl << endl;
}

void PersonPresentation::editComputer(Computer &c) {
    QString sel; // initialized as ""
    while(sel != "5") {
        printEditComputerMenu(c);

        do {
            qout << "Selection: ";
            qout.flush();
            qin >> sel;
        } while(sel != "1" && sel != "2" && sel != "3" && sel != "4" && sel != "5");

        if(sel == "1")
            changeName(c);
        else if(sel == "2")
            changeType(c);
        else if(sel == "3")
            changeYearBuilt(c);
        else if(sel == "4")
            changeConns(c);
    }
}

void PersonPresentation::changeConns(Computer &c) {
    QVector<Person> pList = service.getPersonList();
    printPersonList(pList);
    qout << "Which individual would you like to connect this computer to?" << endl;
    bool success = false;
    uint id;
    do {
        qout << "Selection: ";
        qout.flush();
        QString sel;
        qin >> sel;
        id = utils::stoi(sel, success);
        if(id > pList.last().getId() || !success)
            qout << "Invalid input, try again" << endl;
    } while(id > pList.last().getId() || !success);

    QVector<uint> newConns = c.getConns();
    newConns.append(id);
    c.setConns(newConns);
}

void PersonPresentation::changeName(Computer &c) {
    qout << "New name: ";
    qout.flush();
    qin.read(1);
    QString name = qin.readLine();
    c.setName(name);
}

void PersonPresentation::changeType(Computer &c) {
    qout << "New type: ";
    qout.flush();
    qin.read(1);
    QString type = qin.readLine();
    c.setType(type);
}

void PersonPresentation::changeYearBuilt(Computer &c) {
    qout << "Was it built? [y/n]: ";
    qout.flush();
    QString inp;
    do {
        qin >> inp;
        if(inp != "y" && inp != "n")
            qout << "Invalid choice, try again" << endl;
    } while(inp != "y" && inp != "n");

    if(inp == "y") {
        c.setBuilt(true);
        uint year;
        do {
            qout << "Year built: ";
            qout.flush();
            qin >> year;
            if(!utils::isValidYear(year))
                qout << "Invalid year, try again" << endl;
        } while(!utils::isValidYear(year));

        c.setYearBuilt(year);
    }
    else {
        c.setBuilt(false);
        c.setYearBuilt(-1);
    }
}

void PersonPresentation::editPerson(Person &p) {
    QString sel; // initialized as ""
    while(sel != "6") {
        printEditMenu(p);

        do {
            qout << "Selection: ";
            qout.flush();
            qin >> sel;
        } while(sel != "1" && sel != "2" && sel != "3" && sel != "4" && sel != "5" && sel != "6");

        if(sel == "1")
            changeName(p);
        else if(sel == "2")
            changeGender(p);
        else if(sel == "3")
            changeBirthYear(p);
        else if(sel == "4")
            changeDeathYear(p);
        else if(sel == "5")
            changeConns(p);
    }
}

void PersonPresentation::changeConns(Person &p) {
    QVector<Computer> cList = service.getComputerList();
    printComputerList(cList);
    qout << "Which computer would you like to connect this individual to?" << endl;
    bool success = false;
    uint id;
    do {
        qout << "Selection: ";
        qout.flush();
        QString sel;
        qin >> sel;
        id = utils::stoi(sel, success);
        if(id > cList.last().getId() || !success)
            qout << "Invalid input, try again" << endl;
    } while(id > cList.last().getId() || !success);

    QVector<uint> newConns = p.getConns();
    newConns.append(id);
    p.setConns(newConns);
}

void PersonPresentation::changeName(Person &p) {
    qout << "New name: ";
    qout.flush();
    qin.read(1);
    QString name = qin.readLine();
    p.setName(name);
}

void PersonPresentation::changeGender(Person &p) {
    qout << "New gender: ";
    qout.flush();
    qin.read(1);
    QString gender = qin.readLine();
    p.setGender(gender);
}

void PersonPresentation::changeBirthYear(Person &p) {
    qout << "New year of birth: ";
    qout.flush();
    QString yearStr;
    int year;
    bool success;
    // Get valid year
    do {
        qin >> yearStr;
        year = utils::stoi(yearStr, success);
        if(!success)
            qout << "Invalid year, try again." << endl;
    } while(!success && Person::isValidYear(year));
    p.setBirthYear(year);
}

void PersonPresentation::changeDeathYear(Person &p) {
    qout << "New year of death: ";
    qout.flush();
    QString yearStr;
    int year;
    bool success;
    // Get valid year
    do {
        qin >> yearStr;
        year = utils::stoi(yearStr, success);
        if(!success)
            qout << "Invalid year, try again." << endl;
    } while(!success && Person::isValidDeathYear(year, p.getBirthYear()));
    p.setDeathYear(year);
}

void PersonPresentation::loadInfoFromFile() {
    qout << endl;
    QString fname;

    qout << "Enter file name: ";
    qout.flush();
    qin >> fname;

    if(!service.loadDataFromFile(fname)) {
        qout << "Error opening file. This probably means that the file doesn't exist."
             << endl;
        return;
    }
}

uint PersonPresentation::selectPerson(QString editOrDetails) {
    uint id;

    QVector<Person> pList = service.getPersonList();
    printPersonList(pList);
    QString inp;
    bool success = false;
    // Get valid selection
    do {
        qout << "Which individual would you like " + editOrDetails + "? [enter ID]: ";
        qout.flush();
        qin >> inp;
        id = utils::stoi(inp, success);
        if(!success && id < static_cast<uint>(pList.length()))
            qout << "Invalid input" << endl;
    } while(!success && id < static_cast<uint>(pList.length()));

    return id;
}

uint PersonPresentation::selectComputer(QString editOrDetails) {
    uint id;

    QVector<Computer> cList = service.getComputerList();
    printComputerList(cList);
    QString inp;
    bool success = false;
    // Get valid selection
    do {
        qout << "Which computer would you like " + editOrDetails + "? [enter ID]: ";
        qout.flush();
        qin >> inp;
        id = utils::stoi(inp, success);
        if(!success && id < static_cast<uint>(cList.length()))
            qout << "Invalid input" << endl;
    } while(!success && id < static_cast<uint>(cList.length()));

    return id;
}

void PersonPresentation::printComputerDetails(const Computer c) {
    qout << endl;
    qout << "The " << c.getName() << " is a " << c.getType() << " computer";
    if(c.getBuilt())
        qout << " built in the year " << c.getYearBuilt() << "." << endl;
    else
        qout << " that hasn't been built yet." << endl;

    if(c.getConns().length()) {
        qout << "Scientists that have worked on the " << c.getName() << " include:" << endl;
        for(int i = 0; i < c.getConns().length(); i++) {
            Person p;
            p.setId(c.getConns()[i]);
            if(!service.getPerson(p.getId(), p)) {
                qout << "Error occurred while getting individual info" << endl;
                return;
            }
            qout << p.getName() << endl;
        }
    }
}

void PersonPresentation::printPersonDetails(const Person p) {
    qout << endl;
    qout << p.getName() << ", " << p.getGender() << ",";
    qout << " was born in the year " << p.getBirthYear() << " and ";
    if(p.getDeathYear() < 0)
        qout << "is still alive to this day." << endl;
    else
        qout << "died in the year " << p.getDeathYear() << "." << endl;

    if(p.getConns().length()) {
        qout << "Computers " + p.getName() + " worked on include:" << endl;
        for(int i = 0; i < p.getConns().length(); i++) {
            Computer c;
            c.setId(p.getConns()[i]);
            if(service.getComputer(c).isValid()) {
                qout << "Error occurred while getting computer info" << endl;
                return;
            }
            qout << c.getName() << endl;
        }
    }
    else
        qout << p.getName() << " hasn't worked on any computers." << endl;
}
