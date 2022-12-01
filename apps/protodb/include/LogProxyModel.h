#pragma once

#include <QSortFilterProxyModel>
#include <sol/sol.hpp>

namespace protodb {

class LogProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    LogProxyModel(QObject* parent = nullptr);
    ~LogProxyModel() = default;

    bool setFilterExpression(const QString& str);
    void addNamedConstant(const QString& name, quint32 value);
    void removeNamedConstant(const QString& name);

    void setSourceModel(QAbstractItemModel* model) override;
private:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    void createBaseConstant();

private:
    sol::state m_lua;
    sol::protected_function m_accept;
    bool m_bypass;
};

} // namespace protodb
