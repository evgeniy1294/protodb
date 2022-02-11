#pragma once

#include <QStyledItemDelegate>
#include <protodb/shared_types/LogTypes.h>

class LogItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogItemDelegate(QObject* aParent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    ByteFormat byteFormat() const;
    void setByteFormat(ByteFormat format);

private:
    ByteFormat m_byte_format;
};



