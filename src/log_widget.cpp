#include <QWidget>
#include <QLayout>
#include <QTextBrowser>

#include "log_widget.h"

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
  mLogBrowser = new QTextBrowser();
    mLogBrowser->setText(tr("05.10.21 15:29:53 <<< Connection log"));

  QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(mLogBrowser);
}

