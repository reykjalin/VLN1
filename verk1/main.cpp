#include <QCoreApplication>
#include <QTimer>

#include <iostream>
#include "UI/personpresentation.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PersonPresentation p(&a);

    QObject::connect(&p, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, &p, SLOT(startPresentation()));

    return a.exec();
}
