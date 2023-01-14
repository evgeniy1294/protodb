#pragma once

#include <QStyledItemDelegate>

namespace protodb {

class LogFormatter;
class LogDecorator;

class LogItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogItemDelegate(QObject* aParent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QString message(const QModelIndex& index);

    void setFormatter(LogFormatter* fmt);
    LogFormatter* formatter() const;

    void setDecorator(LogDecorator* dec);
    LogDecorator* decorator() const;

private:
    LogFormatter* m_fmt;
    LogDecorator* m_dec;
};

} // namespace protodb

