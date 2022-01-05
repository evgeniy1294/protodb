#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include <protodb/mainwindow.h>
#include <protodb/Worker.h>
#include <IOWidgetCreatorInterface.h>

#include <protodb/factories/GlobalFactoryStorage.h>
#include <IOWidgetFactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load first plugin
    QPluginLoader loader1("/tmp/protodb_debug/gui/plugins/libSerialIoWidget.so");
    if (auto instance = loader1.instance()) {
        if (auto plugin = qobject_cast<IOWidgetCreatorInterface*>(instance)) {
            qDebug() << "Success!!!";

            GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory());

            auto factory = IOWidgetFactory::globalInstance();
            factory->addCreator(QSharedPointer<IOWidgetCreatorInterface>(plugin));
        }
        else {
            qDebug() << "qobject_cast failed(((";
        }
    }
    else {
        qDebug() << loader1.errorString();
    }

    // load second plugin
    QPluginLoader loader2("/tmp/protodb_debug/gui/plugins/libNetIoWidget.so");
    if (auto instance = loader2.instance()) {
        if (auto plugin = qobject_cast<IOWidgetCreatorInterface*>(instance)) {
            qDebug() << "Success!!!";

            auto factory = IOWidgetFactory::globalInstance();
            factory->addCreator(QSharedPointer<IOWidgetCreatorInterface>(plugin));
        }
        else {
            qDebug() << "qobject_cast failed(((";
        }
    }
    else {
        qDebug() << loader2.errorString();
    }

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.show();

    return a.exec();
}

