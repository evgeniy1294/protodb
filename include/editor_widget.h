#pragma once

#include <QWidget>

class QTextBrowser;
class QLineEdit;
class QLabel;
class QToolButton;
class QMenu;

class EditorWidget: public QWidget
{
  Q_OBJECT

public:
  explicit EditorWidget(QWidget* parent = nullptr);
  ~EditorWidget() = default;

public slots:
  void onSaveClicked();
  void onResetClicked();

private:
  void createActions();
  void createMenu();
  void createGui();

private:
  // ---------[Actions]--------
  QAction* mSaveAct;
  QAction* mResetAct;

  // ----------[Menu]----------
  QMenu* mToolMenu;

  // --------[Widgets]----------
  QTextBrowser* mDataEditor;
  QTextBrowser* mHelpEditor;
  QLineEdit*    mNameLe;
  QLabel*       mStatusLabel;
  QToolButton*  mToolButton;
};
