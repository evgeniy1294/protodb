#include "mainwindow.h"
#include "ProtodbSessionManager.h"

#include <protodb/Worker.h>
#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/utils/MetaTypeUtils.h>

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

void testPlugins();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ProtoDb");

    registerCustomConverters();

    PluginManager::instance().setMainDirectory(QApplication::applicationDirPath() + "/plugins");
    PluginManager::instance().setManualInstallDirectory("/tmp/test/");
    PluginManager::instance().loadPlugins(QMap<QString, bool>());

    //---------- TEST PLUGINS ----------
    testPlugins();
    //----------------------------------

    Worker* worker = new Worker();
    MainWindow w(worker);
        w.restoreState();
        w.showMaximized();

    // -------- TEST SESSIONS ---------
    auto& manager = ProtodbSessionManager::instance();
        manager.setWorkingDirectory("/tmp/protodb/sessions/");
    // --------------------------------

    int ret = a.exec();
        manager.saveCurrentSession();
        manager.saveCurrentState();

    return ret;
}

#include <protodb/plugins/IOWidgetFactory.h>
#include <protodb/plugins/IOWidgetCreatorInterface.h>
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
