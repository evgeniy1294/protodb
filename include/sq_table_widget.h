#pragma once

#include <QTableWidget>

class QAction;
class QToolBar;

class SqTableWidget: public QWidget {
    Q_OBJECT

public:
    explicit SqTableWidget(QWidget *parent = nullptr);
    ~SqTableWidget() = default;

public slots:
    void onClickNew();
    void onClickRemove();
    void onClickClear();

private:
    void createActions();
    void createGui();

private:
    QAction*  mAddAct;
    QAction*  mRemoveAct;
    QAction*  mClearAct;
};


