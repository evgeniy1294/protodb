#include <QApplication>

#include "mainwindow.h"
#include "singleton.h"
#include "Core.h"

void jsonTest();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    jsonTest();

    //MainWindow w;
    //w.restoreState();
    //w.show();

    //Singleton::instance().m_core->init();

    return 0;//a.exec();
}

#include <nlohmann/json.hpp>
#include "LogDecorator.h"

void jsonTest() {
    LogDecorator decorator;

    nlohmann::json json;
    json["LogDecorations"] = nlohmann::json();
    decorator.toJson(json["LogDecorations"]);

    std::cout << json.dump(4);
}
