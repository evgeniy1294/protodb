#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <tools_widget.h>
#include <serial_widget.h>

ToolsWidget::ToolsWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


ToolsWidget::~ToolsWidget()
{

}


void ToolsWidget::createGui()
{
  QLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(new SerialWidget());
}
