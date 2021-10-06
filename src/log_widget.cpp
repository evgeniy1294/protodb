#include <QWidget>
#include <QLayout>
#include <QTextBrowser>

#include "log_widget.h"
#include "editor_widget.h"

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


LogWidget::~LogWidget()
{

}


void LogWidget::createGui()
{
  QGridLayout* layout = new QGridLayout(this);

    mLogBrowser = new QTextBrowser();
    mLogBrowser->setText(tr("05.10.21 15:29:53 <<< Connection log"));

    mScriptBrowser = new QTextBrowser();
    mScriptBrowser->setText(tr("Scripting"));

    layout->addWidget(mLogBrowser, 0, 0, 1, 1);
    layout->addWidget(new EditorWidget(), 1, 0, 1, 1);
    layout->addWidget(mScriptBrowser, 0, 1, 2, 1);
}
