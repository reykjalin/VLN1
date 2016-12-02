#include "personpresentation.h"

void PersonPresentation::startPresentation() {
    bool isOK = true;
    if(!service.startService()) {
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
            qout << endl;
            printPersonList(service.getPersonList());
        }
        else if(input == utils::itos(ADDPERSON)) {
            qin.read(1); // Remove extra newlines in buffer

            qout << endl;
            if(!service.addPerson(createPerson()))
                qout << "This person is already registered in the database."
                     << endl << endl;
            qout << endl;
        }
        else if(input == utils::itos(SEARCH)) {
            qout << endl;

            QVector<Person> found = find();
            if(found.length() == 0)
                qout << "Nothing found." << endl << endl;
            else
                printPersonList(found);
        }
        else if(input == utils::itos(ORDER)) {
            // ORDER
        }
        else
            qout << "Invalid input." << endl;
    }

    if(isOK)
        qout << "Saving new data..." << endl;

    if(!service.closeService())
        qout << "Something went wrong when saving new data to database. " \
                "Please check data integrity" << endl;

    // Close program
    qout << "Closing." << endl;
    emit finished();
}

void PersonPresentation::printMenu() {
    qout << "What do you want to do?" << endl;
    qout << "[" + utils::itos(GETLIST) + "] Get a list of known individuals in " \
            "the history of Computer Science" << endl;
    qout << "[" + utils::itos(ADDPERSON) + "] Add new person" << endl;
    qout << "[" + utils::itos(SEARCH) + "] Search for famous individuals from " \
            "the history of Computer Science" << endl;
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
    QString expression;
    qout << "Search for: ";
    qout.flush();
    qin >> expression;
    if(expression.toLower().contains("alive") ||
       expression.toLower().contains("still"))
    {
        return service.findSimilar(expression) + service.findSimilar("-1");
    }
    return service.findSimilar(expression);
}
