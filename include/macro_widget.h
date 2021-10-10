#pragma once

#include <QWidget>
#include <QList>

class MacroWidget: public QWidget
{
  Q_OBJECT

public:
  explicit MacroWidget(QWidget* parent = nullptr);
  ~MacroWidget();

private:
  void createGui();

private:
  QList<int> mMacroList;
};
