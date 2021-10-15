#include <QLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QToolButton>
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

  /*
  auto macro_wdg = new SequenceMultiWidget();
    connect(&Singleton::instance().mSequenceStorage, &SequenceStorage::sSeqAppended, macro_wdg, &SequenceMultiWidget::addSequence);
    connect(&Singleton::instance().mSequenceStorage, &SequenceStorage::sSeqRemoved, macro_wdg, &SequenceMultiWidget::removeSequence);
    connect(&Singleton::instance().mSequenceStorage, &SequenceStorage::sCleared, macro_wdg, &SequenceMultiWidget::clear);
  */
  /* -------[TEST]---------- */
  auto mSendAct   = new QAction(QIcon(":/icons/arrow.svg"), tr("&Send"), this);
  auto mEditAct   = new QAction(QIcon(":/icons/edit.svg") , tr("&Show in editor"), this);
  auto mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);

  auto mToolMenu = new QMenu();
    mToolMenu->addAction(mSendAct);
    mToolMenu->addAction(mEditAct);
    mToolMenu->addSeparator();
    mToolMenu->addAction(mRemoveAct);

  auto mToolButton  = new QToolButton();
     mToolButton->setIcon(QIcon(":/icons/arrow.svg"));
     mToolButton->setMenu(mToolMenu);

  auto mRepeatTimeWgt = new QSpinBox();
    mRepeatTimeWgt->setMaximum(10000);
    mRepeatTimeWgt->setSingleStep(100);
    mRepeatTimeWgt->setValue(0);
    mRepeatTimeWgt->setSuffix("ms");
    mRepeatTimeWgt->setSpecialValueText(QObject::tr("No Repeat"));

  auto sq_table_wgt = new QTableWidget();
    sq_table_wgt->setColumnCount(4);
    sq_table_wgt->setRowCount(2);

    //sq_table_wgt->horizontalHeader()->setStretchLastSection(true);

    QHeaderView *headerView = new QHeaderView(Qt::Horizontal);
    sq_table_wgt->setHorizontalHeader(headerView);
    headerView->setSectionResizeMode(0, QHeaderView::Stretch);
    headerView->setSectionResizeMode(1, QHeaderView::Stretch);
    headerView->setSectionResizeMode(2, QHeaderView::Fixed);
    headerView->setSectionResizeMode(3, QHeaderView::Fixed);
    sq_table_wgt->setColumnWidth(0, 12);
    sq_table_wgt->horizontalHeader()->hide();

    sq_table_wgt->setCellWidget(0, 2, mRepeatTimeWgt);
    sq_table_wgt->setCellWidget(0, 3, mToolButton);
    sq_table_wgt->setColumnWidth(3, 12);
    //sq_table_wgt->re
    //sq_table_wgt->resizeColumnsToContents();

  auto macro_dock_wdg = new ads::CDockWidget("Macroses");
    macro_dock_wdg->setWidget(sq_table_wgt);


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
