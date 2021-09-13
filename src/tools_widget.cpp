#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <tools_widget.h>
#include <conn_widget.h>

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

    mComboBox = new QComboBox();
      mComboBox->addItem("Connection");
      mComboBox->addItem("Macroses");
      mComboBox->addItem("Scripting");

    auto conn_wdg = new ConnWidget();

    layout->addWidget(mComboBox);
    layout->addWidget(conn_wdg);
}
