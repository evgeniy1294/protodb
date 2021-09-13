#pragma once

#include <QWidget>

class QSplitter;
class QLabel;

class SessionWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SessionWidget(QWidget *parent = nullptr);
  ~SessionWidget();

private:
  void createGui();

private:
  QSplitter* mSplitter;
  QLabel* mLabel;
};
