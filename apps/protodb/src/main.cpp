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
