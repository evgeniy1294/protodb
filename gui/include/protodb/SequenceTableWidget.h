#pragma once

#include <QWidget>

class QDataWidgetMapper;
class QAction;
class QToolBar;
class QMenu;
class QPushButton;
class QLineEdit;
class QSortFilterProxyModel;
class SequenceModel;
class SequenceTableView;
class SequenceEditDialog;

class SequenceTableWidget: public QWidget {
    Q_OBJECT

public:
    explicit SequenceTableWidget(QWidget* parent = nullptr);
    SequenceTableWidget(SequenceModel* model, QWidget* parent = nullptr);
    ~SequenceTableWidget() = default;

    void setSequenceModel(SequenceModel* model);
    SequenceModel* sequenceModel() const;

public slots:
    void onClickRemove();
    void onClickClear();

private:
    void createActions();
    void createGui();
    void createConnections();

private:

    QAction* m_rm_act;
    QAction* m_cp_act;
    QAction* m_ed_act;

    QPushButton* m_add_btn;
    QPushButton* m_rm_btn;
    QPushButton* m_clr_btn;

    QLineEdit* m_search_line;

    QDataWidgetMapper* m_mapper;
    SequenceTableView* m_view;
    QSortFilterProxyModel* m_search_model;
    QMenu* m_menu;

    SequenceModel* m_model;
    SequenceEditDialog* m_edit_dialog;
};


