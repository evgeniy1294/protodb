#pragma once

#include <QWidget>
#include <DockManager.h>


class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget(QWidget *parent = nullptr);
  ~MainWidget() = default;

private:
  void createGui();

private:
  ads::CDockManager* mDockManager;
};
