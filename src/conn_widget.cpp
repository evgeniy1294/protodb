#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include "conn_widget.h"
#include "serial_widget.h"


ConnWidget::ConnWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


ConnWidget::~ConnWidget()
{

}


void ConnWidget::createGui()
{
  QLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);

    mComboBox = new QComboBox();
      mComboBox->addItem("Serial");
      mComboBox->addItem("TCP Server");
      mComboBox->addItem("UDP Server");

    auto serial_wdg = new SerialWidget();

    layout->addWidget(mComboBox);
    layout->addWidget(serial_wdg);
}
