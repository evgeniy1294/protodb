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
    CellButtonDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void setIcon(const QIcon& icon);
    void setAlternateIcon(const QIcon& icon);
    void setText(const QString& text);

    void setCheckable(bool checkable);
    bool isCheckable();

    void setFlat(bool flat);
    bool isFlat();

private:
    QIcon m_icon;
    QIcon m_alt_icon;
    QString m_text;
    bool  m_checkable;
    bool  m_flat;
    int   m_active_row;
};
