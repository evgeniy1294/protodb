#include <QHBoxLayout>
#include <QSplitter>
#include <QTextBrowser>
#include <QLabel>

#include "session_widget.h"
#include "tools_widget.h"
#include "log_widget.h"

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
  auto mLayout = new QHBoxLayout(this);
    auto tools_wdg   = new ToolsWidget();
    auto log_wdg     = new LogWidget();

      mLayout->setAlignment(Qt::AlignTop);
      mLayout->addWidget(tools_wdg, 0);
      mLayout->addWidget(log_wdg, 1);
}
