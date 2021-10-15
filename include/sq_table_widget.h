#pragma once

#include <QTableWidget>
#include "tool_bar.h"

class QAction;

class SqTableWidget: public QWidget {

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
    QAction* mNewAct;
    QAction* mRemoveAct;
    QAction* mClearAct;

    isa_tool_bar* mToolBar;
};
