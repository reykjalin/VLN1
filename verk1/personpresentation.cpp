#include "personpresentation.h"

void PersonPresentation::startPresentation() {
    Person p("Tóti tölvukall", "male", 1990, -1);
    qout << p << endl;

    QString input;
    while(1) {
        // print menu
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
