#pragma once

#include <QWidget>

class QTextBrowser;
class QLineEdit;
class QComboBox;
class QLabel;
class QSpinBox;

class EditorWidget: public QWidget
{
  Q_OBJECT

public:
  explicit EditorWidget(QWidget* parent = nullptr);
  ~EditorWidget();

private:
  void createGui();

private:
  QTextBrowser* mDataEditor;
  QTextBrowser* mHelpEditor;
  QLineEdit* mNameLe;
  QComboBox* mCrcCb;
  QSpinBox* mRepeatSb;
  QLabel* mStatusLabel;
};
