#pragma once

#include <QWidget>

class QTableView;
class QDataWidgetMapper;
class QAction;
class QToolBar;
class QMenu;
class QPushButton;
class SqModel;
class SqTableDialog;
class ButtonDelegate;

class SqTableWidget: public QWidget {
    Q_OBJECT

public:
    explicit SqTableWidget(QWidget *parent = nullptr);
    ~SqTableWidget() = default;

public slots:
    void onClickRemove();
    void onClickClear();

private:
    void createActions();
    void createGui();
    void createConnections();

private:
    QAction* mRemoveAct;
    QAction* mClearAct;
    QAction* mCopyAct;
    QAction* mEditAct;

    QPushButton* mAddBtn;
    QPushButton* mRmBtn;
    QPushButton* mClrBtn;

    ButtonDelegate* mBtnDelegate;

    QDataWidgetMapper* mMapper;
    QTableView* mTblView;
    QMenu* mMenu;

    SqModel* mSqModel;
    SqTableDialog* mDialog;
};


