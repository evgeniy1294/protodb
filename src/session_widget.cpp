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

    mLayout->setAlignment(Qt::AlignTop);
    mLayout->addWidget(new ToolsWidget(), 0);
    mLayout->addWidget(new LogWidget(), 1);
}
