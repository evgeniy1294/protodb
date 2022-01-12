#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include <protodb/mainwindow.h>
#include <protodb/Worker.h>
#include <IOWidgetCreatorInterface.h>

#include <protodb/PluginManagerNew.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <IOWidgetFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("ProtoDb");

    PluginManagerNew::instance().setMainDirectory("/tmp/protodb/gui/plugins/");
    PluginManagerNew::instance().setManualInstallDirectory("/tmp/test/");
    PluginManagerNew::instance().loadPlugins(QMap<QString, bool>());

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.showMaximized();

    return a.exec();
}

