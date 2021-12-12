#include <QListWidget>
#include <QDialogButtonBox>
#include <QLayout>

#include "settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget* aParent)
    : QDialog(aParent)
{
    createGui();
}

void SettingsDialog::createGui()
{
  // --------[BUTTONS]--------- //
  mBtnBox = new QDialogButtonBox( QDialogButtonBox::Ok    | QDialogButtonBox::Apply |
                                  QDialogButtonBox::Reset | QDialogButtonBox::Cancel );

  // ------[LIST WIDGET]------- //
  mWgtSelector = new QListWidget();
      mWgtSelector->addItem(tr("Log window"));
      mWgtSelector->addItem(tr("Scripting"));
      mWgtSelector->addItem(tr("Macroses"));
      mWgtSelector->setResizeMode(QListWidget::Fixed);
      mWgtSelector->setSizeAdjustPolicy(QListWidget::AdjustToContents);

  // ------[CONFIG WIDGETS] ------ //
  auto log_config_widget = new LogConfigWidget();
  mWidgetList.append(log_config_widget);

  // ---------[LAYOUT]---------- //
  auto layout = new QGridLayout();
      layout->addWidget(mWgtSelector, 0 , 0);
      layout->addWidget(log_config_widget,0 ,1);
      layout->addWidget(mBtnBox, 1, 0, 1, 2);
      layout->setColumnStretch(0, 0);
      layout->setColumnStretch(1, 1);

  setLayout(layout);
}
