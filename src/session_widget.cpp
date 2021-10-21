#include <QLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <DockManager.h>

#include "session_widget.h"
#include "sq_table_widget.h"
#include "serial_widget.h"
#include "log_widget.h"
#include "script_widget.h"
#include "sq_model.h"


SessionWidget::SessionWidget(QWidget *parent)
  : QWidget(parent)
  , mDockManager(new ads::CDockManager())
{
  createGui();
}



SessionWidget::~SessionWidget()
{

}



void SessionWidget::createGui()
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);

    auto serial_dock_wdg = new ads::CDockWidget("Connection");
        serial_dock_wdg->setWidget(new SerialWidget());

    auto log_dock_wdg = new ads::CDockWidget("Log");
        log_dock_wdg->setWidget(new LogWidget());

    auto script_dock_wdg = new ads::CDockWidget("Scripting");
        script_dock_wdg->setWidget(new ScriptWidget());

    auto macro_dock_wdg = new ads::CDockWidget("Macroses");
        macro_dock_wdg->setWidget(new SqTableWidget());

    mDockManager->addDockWidget(ads::LeftDockWidgetArea, serial_dock_wdg);
    mDockManager->addDockWidget(ads::RightDockWidgetArea, log_dock_wdg);
    mDockManager->addDockWidget(ads::RightDockWidgetArea, script_dock_wdg);
    mDockManager->addDockWidgetTab(ads::RightDockWidgetArea, macro_dock_wdg);

    QHBoxLayout *central = new QHBoxLayout();
        central->addWidget( mDockManager );

    setLayout(central);
}



