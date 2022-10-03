#include "MainClass.h"
#include "mainwindow.h"
#include "ProtodbSessionManager.h"

#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/utils/MetaTypeUtils.h>

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

void testPlugins();
void testZip();

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

    auto& main_class = MainClass::instance();
        main_class.init();

    MainWindow w;
        w.restoreState();
        w.showMaximized();

    auto& session_manager = ProtodbSessionManager::instance();
        session_manager.setWorkingDirectory("/tmp/protodb/sessions/");

    int ret = a.exec();

    session_manager.saveCurrentSession();
    session_manager.saveCurrentState();

    return ret;
}

#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/factories/IODeviceFactory.h>
#include <protodb/creators/IOWidgetCreatorInterface.h>
#include <protodb/creators/IODeviceCreatorInterface.h>
void testPlugins() {
    // IOWidget
    auto iowgt_factory = IOWidgetFactory::globalInstance();
    if (!iowgt_factory) {
        GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory);
        iowgt_factory = IOWidgetFactory::globalInstance();
    }

    auto iowgt_creators = PluginManager::instance().getPlugins<IOWidgetCreatorInterface>();

    for (auto& it: iowgt_creators) {
        iowgt_factory->addCreator(QSharedPointer<IOWidgetCreatorInterface>(it));
    }

    // IODevice
    auto iodev_factory = IODeviceFactory::globalInstance();
    if (!iodev_factory) {
        GlobalFactoryStorage::addFactory(IODeviceFactory::fid(), new IODeviceFactory);
        iodev_factory = IODeviceFactory::globalInstance();
    }

    auto iodev_creators = PluginManager::instance().getPlugins<IODeviceCreatorInterface>();
    for (auto& it: iodev_creators) {
        iodev_factory->addCreator(QSharedPointer<IODeviceCreatorInterface>(it));
    }

}
