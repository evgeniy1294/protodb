#include <QLayout>
#include <DockManager.h>

#include "session_widget.h"
#include "serial_widget.h"
#include "log_widget.h"
#include "editor_widget.h"
#include "script_widget.h"
#include "macro_widget.h"


SessionWidget::SessionWidget(QWidget *parent)
  : QWidget(parent)
{
  createGui();
}



SessionWidget::~SessionWidget()
{

}



void SessionWidget::createGui()
{
  auto serial_wdg      = new SerialWidget();
  auto serial_dock_wdg = new ads::CDockWidget("Connection");
    serial_dock_wdg->setWidget(serial_wdg);

  auto log_wdg      = new LogWidget();
  auto log_dock_wdg = new ads::CDockWidget("Log");
    log_dock_wdg->setWidget(log_wdg);

  auto edit_wdg      = new EditorWidget();
  auto edit_dock_wdg = new ads::CDockWidget("Editor");
    edit_dock_wdg->setWidget(edit_wdg);

  auto script_wdg      = new ScriptWidget();
  auto script_dock_wdg = new ads::CDockWidget("Scripting");
    script_dock_wdg->setWidget(script_wdg);

  auto macro_wdg = new SequenceMultiWidget();
    Sequence seq;
    seq.setName(tr("Sequence 1"));
    seq.setTriggerName(tr("Sequence 2"));
    macro_wdg->addMacroSequence(&seq);

    seq.setName(tr("Sequence 2"));
    seq.setTriggerName(tr("None"));
    macro_wdg->addMacroSequence(&seq);

    seq.setName(tr("Sequence 3"));
    seq.setTriggerName(tr("None"));
    macro_wdg->addMacroSequence(&seq);

  auto macro_dock_wdg = new ads::CDockWidget("Macroses");
    macro_dock_wdg->setWidget(macro_wdg);

  mDockManager = new ads::CDockManager();
    mDockManager->addDockWidget(ads::LeftDockWidgetArea, serial_dock_wdg);
    mDockManager->addDockWidget(ads::RightDockWidgetArea, log_dock_wdg);
    mDockManager->addDockWidget(ads::BottomDockWidgetArea, edit_dock_wdg);
    mDockManager->addDockWidget(ads::RightDockWidgetArea, script_dock_wdg);
    mDockManager->addDockWidgetTab(ads::RightDockWidgetArea, macro_dock_wdg);

    QHBoxLayout *central = new QHBoxLayout();
        central->setContentsMargins( 0, 0, 0, 0 );
        central->addWidget( mDockManager );

    setLayout(central);
}
