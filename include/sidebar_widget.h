#pragma once

#include <QWidget>

class QAction;

class SidebarWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SidebarWidget(QWidget* parent = nullptr);
  ~SidebarWidget();

private:
  void createGui();

private:
    // QComboBox* mComboBox;
};
