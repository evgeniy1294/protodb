#pragma once

#include <QWidget>

class QTableView;
class QDataWidgetMapper;
class QAction;
class QToolBar;
class QMenu;
class QPushButton;
class QLineEdit;
class QSortFilterProxyModel;
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

    QLineEdit* mFindLe;

    QDataWidgetMapper* mMapper;
    QTableView* mTblView;
    QSortFilterProxyModel* mFilter;
    QMenu* mMenu;

    ButtonDelegate* mBtnDelegate;
    SqModel* mSqModel;
    SqTableDialog* mDialog;
};


