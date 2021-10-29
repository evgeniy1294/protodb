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
class OutSqTableModel;
class SqTableDialog;
class CellButtonDelegate;

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

    CellButtonDelegate* mBtnDelegate;
    OutSqTableModel* mSqModel;
    SqTableDialog* mDialog;
};


