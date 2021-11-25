#include <QHeaderView>
#include "LogTableView.h"
#include "LogItemDelegate.h"
#include "LogModel.h"

LogTableView::LogTableView(QWidget *parent)
    : QTableView(parent)
{
    setItemDelegate(new LogItemDelegate());
    setWordWrap(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);

    QHeaderView* vh = verticalHeader();
        vh->setSectionResizeMode(QHeaderView::ResizeToContents);
        vh->hide();

    QHeaderView* hh = horizontalHeader();
        hh->hide();
}

void LogTableView::setModel(QAbstractItemModel *model)
{
    if (dynamic_cast<LogModel*>(model)) {
        QTableView::setModel(model);
        QHeaderView* hh = horizontalHeader();
            hh->setSectionResizeMode(LogModel::kColumnTimestamp, QHeaderView::ResizeToContents);
            hh->setSectionResizeMode(LogModel::kColumnChannel,   QHeaderView::ResizeToContents);
            hh->setSectionResizeMode(LogModel::kColumnMsg,       QHeaderView::Stretch);
    }
}

void LogTableView::setByteFormat(ByteFormat format)
{
    Q_UNUSED(format);
}
