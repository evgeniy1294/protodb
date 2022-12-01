#pragma once

#include <QTableView>

namespace protodb {

class CellButtonDelegate;
class CellCheckBoxDelegate;
class CellSpinBoxDelegate;

class SequenceTableView: public QTableView
{
    Q_OBJECT

public:
    SequenceTableView(QWidget* parent = nullptr);
    void setSequenceModel(QAbstractItemModel *model, bool incomingMode);

private:
    CellButtonDelegate*   m_btn_delegate;
    CellCheckBoxDelegate* m_checkbox_delegate;
    CellSpinBoxDelegate*  m_spinbox_delegate;
};

} // namespace protodb
