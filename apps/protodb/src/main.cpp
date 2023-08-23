#include "MainClass.h"
#include "mainwindow.h"
#include "ProtodbSessionManager.h"
#include "Sequence.h"

#include <ProtodbConfigStorage.h>
#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/utils/MetaTypeUtils.h>

#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

using namespace protodb;

void registerMetaType();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        a.setStyle("Fusion");
        a.setWindowIcon(QIcon(":/icons/terminal.svg"));
    QCoreApplication::setApplicationName("protodb");

    // TODO: перенести ресурсы в отдельную библиотеку. После чего убрать set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) для gui
    Q_INIT_RESOURCE(gui_resources);

    registerMetaType();
    registerCustomConverters();

    ProtodbConfigStorage::instance().loadConfig();
        auto userPluginsLocation = ProtodbConfigStorage::instance().userPluginsLocation();
        auto sessionsLocation = ProtodbConfigStorage::instance().sessionsLocation();
        auto pluginsState = ProtodbConfigStorage::instance().lastPluginsState();

    #ifdef _WIN32
        PluginManager::instance().setMainDirectory(QCoreApplication::applicationDirPath() + "/plugins");
    #else
        //PluginManager::instance().setMainDirectory("/usr/lib/protodb/plugins");
        PluginManager::instance().setMainDirectory("/home/evgen/Workspace/Install/protodb/lib/protodb/plugins/");
    #endif

    PluginManager::instance().setManualInstallDirectory(userPluginsLocation);
    PluginManager::instance().loadPlugins(pluginsState);

    auto& main_class = MainClass::instance();
        main_class.init();

    MainWindow* w = new MainWindow();
        w->showMaximized();

    auto& session_manager = ProtodbSessionManager::instance();
        session_manager.setWorkingDirectory(sessionsLocation);

    int ret = a.exec();

    ProtodbConfigStorage::instance().saveConfig();

    return ret;
}

void registerMetaType() {
    qRegisterMetaType<protodb::Sequence>();
    qRegisterMetaType< QSharedPointer<protodb::Sequence> >();
}
