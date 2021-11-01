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
class SequenceTableModel;
class SqTableDialog;

class SqTableWidget: public QWidget {
    Q_OBJECT

public:
    enum DisplayMode {
        kIncomingDisplayMode,
        kOutgoingDisplayMode
    };

public:
    explicit SqTableWidget(DisplayMode mode, QWidget* parent = nullptr);
    ~SqTableWidget() = default;

public slots:
    void onClickRemove();
    void onClickClear();

private:
    void createActions();
    void createGui();
    void createConnections();

private:
    const DisplayMode m_mode;

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

    SequenceTableModel* mSqModel;
    SqTableDialog* mDialog;
};


