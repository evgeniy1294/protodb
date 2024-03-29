#include "LogProxyModel.h"

#include "Logger.h"
#include <protodb/utils/SolByteArrayWrapper.h>

using namespace protodb;

LogProxyModel::LogProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_bypass(true)
{
    createBaseConstant();
}

void LogProxyModel::createBaseConstant()
{
    m_lua["RX"]  = Logger::ChannelFirst;
    m_lua["TX"]  = Logger::ChannelSecond;
    m_lua["COM"] = Logger::ChannelComment;
    m_lua["ERR"] = Logger::ChannelError;
}


bool LogProxyModel::setFilterExpression(const QString& str)
{
    static const QByteArray checkArray(5, '0');
    static const QString filterTemplate = {
        "function accept(channel, msg) \n return %1 \n end"
    };

    bool ret = true;

    if (str.length() != 0) {
        sol::protected_function_result pfr =
                m_lua.safe_script(filterTemplate.arg(str).toStdString(), &sol::script_pass_on_error);

        ret = pfr.valid();

        if (pfr.valid()) {
            pfr = m_lua["accept"](0, checkArray); // Нужно тестировать на все каналы?
            ret = pfr.get_type() == sol::type::boolean;

            m_accept = m_lua["accept"];
        }

        m_bypass = !ret;
    }
    else {
        m_bypass = true;
    }

    invalidate();

    return ret;
}

void LogProxyModel::addNamedConstant(const QString& name, quint32 value)
{
}

void LogProxyModel::removeNamedConstant(const QString& name)
{
}

void LogProxyModel::setSourceModel(QAbstractItemModel* model)
{
    auto logger = qobject_cast<Logger*>(model);
    if (logger) {
        QSortFilterProxyModel::setSourceModel(logger);
    }

    return;
}

bool LogProxyModel::filterAcceptsColumn(int source_column, const QModelIndex& source_parent) const
{
    return true;
}

bool LogProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    bool ret = true;

    if (!m_bypass) {
        sol::protected_function_result pfr;

        auto model = sourceModel();

        auto channel = model->data( model->index(source_row, Logger::ColumnChannel) ).toInt();
        auto msg     = model->data( model->index(source_row, Logger::ColumnMsg) ).toByteArray();

        pfr = m_accept(channel, SolByteArrayWrapper(msg));
        if (pfr.valid()) {
            if (pfr.get_type() == sol::type::boolean) {
                ret = pfr.get<bool>();
            }
        }
    }

    return ret;
}



