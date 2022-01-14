#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include <protodb/mainwindow.h>
#include <protodb/Worker.h>
#include <IOWidgetCreatorInterface.h>

#include <protodb/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <IOWidgetFactory.h>

void testPlugins();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ProtoDb");

    PluginManager::instance().setMainDirectory("/tmp/protodb/gui/plugins/");
    PluginManager::instance().setManualInstallDirectory("/tmp/test/");
    PluginManager::instance().loadPlugins(QMap<QString, bool>());

    //---------- TEST PLUGINS ----------
    testPlugins();
    //----------------------------------

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.showMaximized();

    return a.exec();
}

void testPlugins() {
    auto factory = IOWidgetFactory::globalInstance();
    if (!factory) {
        GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory);
        factory = IOWidgetFactory::globalInstance();
    }


    auto creators = PluginManager::instance().getPlugins<IOWidgetCreatorInterface>();

    for (auto& it: creators) {
        factory->addCreator(QSharedPointer<IOWidgetCreatorInterface>(it));
    }

}
