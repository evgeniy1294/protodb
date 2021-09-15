#include <QLayout>
#include <QTextBrowser>
#include "workarea_widget.h"

WorkareaWidget::WorkareaWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


WorkareaWidget::~WorkareaWidget()
{

}


void WorkareaWidget::createGui()
{
  auto v_layout = new QVBoxLayout(this);
    auto send_tb = new QTextBrowser();
    auto h_layout = new QHBoxLayout(this);
      auto message_tb = new QTextBrowser();
      auto scripting_tb = new QTextBrowser();
    h_layout->addWidget(message_tb);
    h_layout->addWidget(scripting_tb);


  v_layout->addLayout(h_layout);
  v_layout->addWidget(send_tb);
}

