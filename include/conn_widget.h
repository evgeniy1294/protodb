#pragma once

#include <QWidget>

class QComboBox;

class ConnWidget: public QWidget
{
  Q_OBJECT

public:
  explicit ConnWidget(QWidget* parent = nullptr);
  ~ConnWidget();

private:
  void createGui();

private:
  QComboBox* mComboBox;
};
 
