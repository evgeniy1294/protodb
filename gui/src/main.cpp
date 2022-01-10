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
    QCoreApplication::setApplicationName("ProtoDb");

    PluginManager::instance().addDirectory("/tmp/protodb_debug/gui/plugins/");
    PluginManager::instance().setBaseDirectory("/tmp/protodb_debug/gui/plugins/");
    PluginManager::instance().setManualInstallDirectory("/tmp/test/");
    PluginManager::instance().loadPlugins();

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.showMaximized();

    return a.exec();
}

