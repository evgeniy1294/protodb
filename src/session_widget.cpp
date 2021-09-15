#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include "session_widget.h"
#include "tools_widget.h"
#include "workarea_widget.h"


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

  mLayout->addWidget(new ToolsWidget(), 0);
  mLayout->addWidget(new WorkareaWidget(), 1);
}
