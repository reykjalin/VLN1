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
                SEARCH, "Search for famous individuals from the history of Computer Science");
    selectionDescriptions.insert(
                ORDER, "Choose the order in which the list of individuals will appear");
    selectionDescriptions.insert(
                LOADFILE, "Import data from external file");
}

void PersonPresentation::startPresentation() {
    // Skip loop and don't save data if isOK becomes false
    bool isOK = true;
    if(!service.startService()) {
        qout << "Something went wrong when accessing saved data." << endl;
        isOK = false;
    }

    // Only sort if everything is a-OK
    if(isOK)
        service.sort(); // Sort initial list

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
            service.sort(); // Sort after adding to list

        }
        else if(input == utils::itos(SEARCH)) {

            QVector<Person> found = find();
            if(found.length() == 0)
                qout << "Nothing found." << endl;
            else
                printPersonList(found);

        }
        else if(input == utils::itos(ORDER)) {

            sort();         // Configure sort
            service.sort(); // Perform the actual sort
            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(LOADFILE)) {

            loadInfoFromFile();
            service.sort();      // Sort new data
            printPersonList(service.getPersonList());

        }
        else if(input == utils::itos(EDITPERSON)) {

            editPerson();
            service.sort();
            printPersonList(service.getPersonList());

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
    int longestName   = 4;  // length of string "Name"
    int longestGender = 5;  // length of string "other"
    int yearOfBirth   = 13; // length of string "year of birth"
    // get longest id
    int longestId     = utils::itos(pList.length()).length();
    longestId = (longestId < 2 ? 2 : longestId);

    findLongestNameAndGender(longestName, longestGender, pList);

    qout << endl;
    printSeperator(longestName, longestGender, longestId);
    printListHeader(longestName, longestGender, longestId);
    int i;
    for(i = 0; i < pList.length(); i++) {
        qout << "|";
        printSpacing(longestId - utils::itos(i).length());
        qout << " " + utils::itos(i) + " ";

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

QVector<Person> PersonPresentation::find() {
    qout << endl;
    QString expression;
    qout << "Search for: ";
    qout.flush();

    qin.read(1);            // Clear input buffer
    expression = qin.readLine();
    if(expression.toLower().contains("alive") ||
       expression.toLower().contains("still"))
    {
        return service.findSimilar(expression) + service.findSimilar("-1");
    }
    return service.findSimilar(expression);
}

void PersonPresentation::sort() {
    QString sort;
    printSortMenu();
    qin >> sort;
    if(sort == utils::itos(utils::NAME))
        service.setSort(utils::NAME);
    else if(sort == utils::itos(utils::GENDER))
        service.setSort(utils::GENDER);
    else if(sort == utils::itos(utils::BIRTH))
        service.setSort(utils::BIRTH);
    else if(sort == utils::itos(utils::DEATH))
        service.setSort(utils::DEATH);
    qout << endl;
}

void PersonPresentation::printSortMenu() {
    qout << endl;
    qout << "[" + utils::itos(utils::NAME)   + "] Name"          << endl;
    qout << "[" + utils::itos(utils::GENDER) + "] Gender"        << endl;
    qout << "[" + utils::itos(utils::BIRTH)  + "] Year of birth" << endl;
    qout << "[" + utils::itos(utils::DEATH)  + "] Year of death" << endl;
    qout << "Any other selection will bring you back to the main menu" << endl << endl;
    qout << "Sort by: ";
    qout.flush();
}

void PersonPresentation::editPerson() {
    QVector<Person> pList = service.getPersonList();
    printPersonList(pList);
    QString inp;
    int id;
    bool success = false;
    // Get valid selection
    do {
        qout << "Which individual would you like to edit? [enter ID]: ";
        qout.flush();
        qin >> inp;
        id = utils::stoi(inp, success);
        if(!success)
            qout << "Invalid input" << endl;
    } while(!success && id >= 0 && id < pList.length());

    Person toEdit;
    // Get person info
    service.getPerson(id, toEdit);
    // Edit info
    editPerson(toEdit);
    // Edit info in DB
    service.editPerson(id, toEdit);
}

void PersonPresentation::printEditMenu(Person p) {
        qout << endl;
        qout << "Editing: " << endl;
        qout << p << endl;

        qout << "Which do you want to change?" << endl;
        qout << "1. Name"                      << endl;
        qout << "2. Gender"                    << endl;
        qout << "3. Year of birth"             << endl;
        qout << "4. Year of death"             << endl;
        qout << "5. Done, return to main menu" << endl << endl;
}

void PersonPresentation::editPerson(Person &p) {
    QString sel; // initialized as ""
    while(sel != "5") {
        printEditMenu(p);

        do {
            qout << "Selection: ";
            qout.flush();
            qin >> sel;
        } while(sel != "1" && sel != "2" && sel != "3" && sel != "4" && sel != "5");

        if(sel == "1")
            changeName(p);
        else if(sel == "2")
            changeGender(p);
        else if(sel == "3")
            changeBirthYear(p);
        else if(sel == "4")
            changeDeathYear(p);
    }
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
