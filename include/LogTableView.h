#pragma once

#include <QTableView>
#include "logging.h"

class LogItemDelegate;

class LogTableView: public QTableView
{
    Q_OBJECT

public:
    LogTableView(QWidget* parent = nullptr);
    void setModel(QAbstractItemModel *model) override;
    void setByteFormat(ByteFormat format);
};
