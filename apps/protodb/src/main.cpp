#include "MainClass.h"
#include "mainwindow.h"
#include "ProtodbSessionManager.h"
#include "Sequence.h"

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
    QCoreApplication::setApplicationName("protodb");

    registerMetaType();
    registerCustomConverters();

    PluginManager::instance().setMainDirectory("/tmp/protodb/install/lib/protodb/plugins");
    PluginManager::instance().setManualInstallDirectory("/tmp/test/");
    PluginManager::instance().loadPlugins(QMap<QString, bool>());

    auto& main_class = MainClass::instance();
        main_class.init();

    protodb::MainWindow w;
        w.restoreState();
        w.showMaximized();

    auto& session_manager = ProtodbSessionManager::instance();
        session_manager.setWorkingDirectory("/tmp/protodb/sessions/");

    int ret = a.exec();

    session_manager.saveCurrentSession();
    session_manager.saveCurrentState();

    return ret;
}

void registerMetaType() {
    qRegisterMetaType<protodb::Sequence>();
    qRegisterMetaType< QSharedPointer<protodb::Sequence> >();
}
