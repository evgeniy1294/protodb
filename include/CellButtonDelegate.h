#pragma once

#include <QStyledItemDelegate>

class CellButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum Role {
        kClickRole = Qt::UserRole,
    };

public:
    CellButtonDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void setIcon(const QIcon& icon);
    void setCheckedIcon(const QIcon& icon);
    void setUncheckedIcon(const QIcon& icon);
    void setText(const QString& text);

    void setCheckable(bool checkable);
    bool isCheckable();

    void setFlat(bool flat);
    bool isFlat();

signals:
    void triggered(const QModelIndex &index);

private:
    bool  m_flat;
    bool  m_checkable;
    int   m_active_row;
    QIcon m_icon;
    QIcon m_checked_icon;
    QString m_text;
};
