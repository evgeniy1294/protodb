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
#include <protodb/creators/IOWidgetCreatorInterface.h>
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
