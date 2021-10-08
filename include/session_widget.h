#pragma once

#include <QWidget>

namespace ads {
  class CDockManager;
}

class SessionWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SessionWidget(QWidget *parent = nullptr);
  ~SessionWidget();

private:
  void createGui();

private:
  ads::CDockManager* mDockManager;
};
