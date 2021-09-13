#pragma once

#include <QWidget>

class QComboBox;

class ToolsWidget: public QWidget
{
  Q_OBJECT

public:
  explicit ToolsWidget(QWidget* parent = nullptr);
  ~ToolsWidget();

private:
  void createGui();

private:
  QComboBox* mComboBox;
};
