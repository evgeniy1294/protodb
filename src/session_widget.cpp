#include <QLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <DockManager.h>

#include "session_widget.h"
#include "sq_table_widget.h"
#include "LogWidget.h"
#include "script_widget.h"


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

    auto log_dock_wdg = new ads::CDockWidget("Log");
        log_dock_wdg->setWidget(new LogWidget());

    auto script_dock_wdg = new ads::CDockWidget("Scripting");
        script_dock_wdg->setWidget(new ScriptWidget());

    auto seq_inc_dock_wdg = new ads::CDockWidget("Incoming");
        seq_inc_dock_wdg->setWidget(new SqTableWidget(SqTableWidget::kIncomingDisplayMode));

    auto seq_out_dock_wdg = new ads::CDockWidget("Outgoing");
        seq_out_dock_wdg->setWidget(new SqTableWidget(SqTableWidget::kOutgoingDisplayMode));

    mDockManager->addDockWidget(ads::RightDockWidgetArea, log_dock_wdg);
    mDockManager->addDockWidgetTab(ads::RightDockWidgetArea, script_dock_wdg);
    mDockManager->addDockWidget(ads::RightDockWidgetArea, seq_out_dock_wdg);
    mDockManager->addDockWidgetTab(ads::BottomDockWidgetArea, seq_inc_dock_wdg);

    QHBoxLayout *central = new QHBoxLayout();
        central->addWidget( mDockManager );

    setLayout(central);
}



