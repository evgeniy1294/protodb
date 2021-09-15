#pragma once

#include <QWidget>

class WorkareaWidget: public QWidget
{
Q_OBJECT

public:
  explicit WorkareaWidget(QWidget* parent = nullptr);
  ~WorkareaWidget();

private:
  void createGui();

private:

};
