#pragma once

#include <QTableWidget>

class QAction;
class QDataWidgetMapper;
class QToolBar;
class SqModel;
class SqTableDialog;

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

private:
    QAction*  mRemoveAct;
    QAction*  mClearAct;
    QDataWidgetMapper* mMapper;
    SqModel* mSqModel;
    SqTableDialog* mDialog;
};


