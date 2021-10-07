#include "main_widget.h"
#include "session_widget.h"

MainWidget::MainWidget(QWidget* parent)
  : QWidget(parent)
  , mDockManager(new ads::CDockManager(this))
{
  createGui();
}

void MainWidget::createGui()
{
  SessionWidget* session_wdg = new SessionWidget();
  ads::CDockWidget* dock_wdg = new ads::CDockWidget("sessionName");
  dock_wdg->setWidget(session_wdg);

  mDockManager->addDockWidget(ads::TopDockWidgetArea, dock_wdg);
}






