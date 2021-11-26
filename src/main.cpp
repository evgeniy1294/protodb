#include <QApplication>

#include "mainwindow.h"
#include "singleton.h"
#include "Worker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.show();

    return a.exec();
}

