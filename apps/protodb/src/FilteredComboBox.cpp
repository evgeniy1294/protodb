#include "FilteredComboBox.h"

#include <QSortFilterProxyModel>
#include <QCompleter>
#include <QKeyEvent>

protodb::FilteredComboBox::FilteredComboBox(QWidget* parent)
    : QComboBox(parent)
{
    setMaxVisibleItems(10);
    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);
    setStyleSheet("combobox-popup: 0;");

    m_proxy = new QSortFilterProxyModel(this);
        m_proxy->setSourceModel(model());
        m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        m_proxy->setFilterKeyColumn(modelColumn());

    auto completer = new QCompleter(this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setModel(m_proxy);
        completer->setCompletionColumn(modelColumn());
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    setCompleter(completer);

    installEventFilter(this);
}

bool protodb::FilteredComboBox::eventFilter(QObject* watched, QEvent* event)
{
   if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        auto k = keyEvent->key();
        if (k < Qt::Key_Escape || k == Qt::Key_Enter || k == Qt::Key_Delete || k == Qt::Key_Backspace) {
            m_proxy->setFilterFixedString(currentText());
            return true;
        }
    }

    return QComboBox::eventFilter(watched, event);
}
