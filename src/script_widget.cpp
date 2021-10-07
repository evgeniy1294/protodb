#include <QWidget>
#include <QLayout>
#include <QTextBrowser>

#include "script_widget.h"

ScriptWidget::ScriptWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


ScriptWidget::~ScriptWidget()
{

}


void ScriptWidget::createGui()
{
  mScriptBrowser = new QTextBrowser();
    mScriptBrowser->setText(tr("Scripting"));

  QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(mScriptBrowser);
}

