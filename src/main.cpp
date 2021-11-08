#include <QApplication>

#include "mainwindow.h"
#include "singleton.h"
#include "Core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    MainWindow w;
    w.restoreState();
    w.show();

    Singleton::instance().m_core->init();

    return a.exec();
}
