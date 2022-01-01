#pragma once

#include <QWidget>
#include <QFrame>

class QDialogButtonBox;
class QPushButton;
class QFontComboBox;
class QComboBox;

class LogConfigWidget: public QFrame
{
    Q_OBJECT

public:
    LogConfigWidget(QWidget* aParent = nullptr);
    ~LogConfigWidget() = default;

private slots:
    void colorDialog();

private:
    void createGui();
    void connectSignals();

private:
    QDialogButtonBox* mButtonBox;
    QFontComboBox* mFontCb;
    QComboBox* mFontSizeCb;
    QPushButton* mCh1ColorBtn;
    QPushButton* mCh2ColorBtn;
    QPushButton* mChSrvColorBtn;
    QPushButton* mChUsrColorBtn;
    QPushButton* mBkgColorBtn;
};
