#include "mainwindow.h"

#include <QApplication>
#include "singleton.h"
#include "Core.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();

  Singleton::instance().m_core->init();

  return a.exec();
}
