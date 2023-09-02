#pragma once

#include <QComboBox>
#include <QListView>

class QSortFilterProxyModel;

namespace protodb {

class FilteredComboBox final: public QComboBox
{
    Q_OBJECT

public:
    FilteredComboBox(QWidget* parent = nullptr);
   ~FilteredComboBox() = default;

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

    QSortFilterProxyModel* m_proxy;
};


} // namespace protodb
