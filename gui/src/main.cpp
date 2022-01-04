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
    QPluginLoader loader("/tmp/protodb_debug/gui/plugins/libSerialIoWidget.so");
    if (auto instance = loader.instance()) {
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
        qDebug() << loader.errorString();
    }

    QCoreApplication::setOrganizationName("protodb");
    QCoreApplication::setApplicationName("protodb");

    Worker* worker = new Worker();
    MainWindow w(worker);
    w.restoreState();
    w.show();

    return a.exec();
}

