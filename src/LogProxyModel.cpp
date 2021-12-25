#include "LogProxyModel.h"
#include "LogModel.h"
#include "SolByteArrayWrapper.h"

#include <iostream>

LogProxyModel::LogProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    m_lua.open_libraries(sol::lib::base);
    createBaseConstant();
}

void LogProxyModel::createBaseConstant()
{
    m_lua.script(R"(
        function bypass(channel, msg)
          return true
        end
    )");

    m_accept = m_lua["bypass"];

    m_lua["RX"]  = kFirstLogChannel;
    m_lua["TX"]  = kSecondLogChannel;
    m_lua["LUA"] = kCommentLogChannel;
    m_lua["ERR"] = kErrorLogChannel;
}

bool LogProxyModel::setFilterExpression(const QString& str)
{
    static const std::array<int, 5> checkArray = {};
    static const QString filterTemplate = {
        "function accept(channel, msg) \n return %1 \n end"
    };

    bool ret = false;

    sol::protected_function_result pfr =
            m_lua.safe_script(filterTemplate.arg(str).toStdString(), &sol::script_pass_on_error);

    if (pfr.valid()) {
        pfr = m_lua["accept"](0, checkArray);

        if (pfr.get_type() == sol::type::boolean) {
            ret = true;
        }
    }

    m_accept = ret ? m_lua["accept"] : m_lua["bypass"];
    invalidate();

    return ret;
}

void LogProxyModel::addFilterConstant(const QString& name, quint32 value)
{

}

void LogProxyModel::setSourceModel(QAbstractItemModel* model)
{
    if (dynamic_cast<LogModel*>(model)) {
        QSortFilterProxyModel::setSourceModel(model);
    }
}

bool LogProxyModel::filterAcceptsColumn(int source_column, const QModelIndex& source_parent) const
{
    return true;
}

bool LogProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    auto index   = sourceModel()->index(source_row, 0);
    auto channel = index.data(LogModel::ChannelRole).toInt();
    auto data    = index.data(LogModel::AnalyzeRole).toByteArray();
        SolByteArrayWrapper dataWrapper(data);

     sol::protected_function_result pfr = m_accept(channel, dataWrapper);

     return pfr.get<bool>();
}





