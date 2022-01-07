#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include <protodb/mainwindow.h>
#include <protodb/Worker.h>
#include <IOWidgetCreatorInterface.h>

#include <protodb/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <IOWidgetFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PluginManager::instance().addDirectory("/tmp/protodb_debug/gui/plugins/");
    PluginManager::instance().loadPlugins();

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.show();

    return a.exec();
}

