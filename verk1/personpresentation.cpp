#include "personpresentation.h"

void PersonPresentation::startPresentation() {
    Person p("Tóti tölvukall", "male", 1990, -1);
    qout << p << endl;

    QString input;
    while(1) {
        printMenu();

        qin >> input;
        if(input == "q")
            break;

        if(input == "1") {
            // Do stuff
            qout << "do stuff" << endl;
        }

        qout << "Invalid input." << endl;
    }
    qout << "Closing." << endl;
    emit finished();
}

void PersonPresentation::printMenu() {
    qout << "What do you want to do?" << endl;
    qout << "[1] Get a list of known individuals in the history of Computer Science" << endl;
    qout << "[2] Search for famous individuals from the history of Computer Science" << endl;
    qout << "[q] Quit" << endl;
}
