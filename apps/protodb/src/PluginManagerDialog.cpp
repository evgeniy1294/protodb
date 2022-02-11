#include "PluginTreeView.h"
#include "PluginManagerDialog.h"

#include <protodb/plugins/PluginManager.h>

#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QGridLayout>
#include <QSortFilterProxyModel>
#include <QAbstractButton>


PluginManagerDialog::PluginManagerDialog(QWidget* parent)
    : QDialog(parent)
{
    createGui();
    connectSignals();

    resize(640, 480);
    setWindowTitle("Plugins");
}

void PluginManagerDialog::createGui()
{
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok |
                                             QDialogButtonBox::Apply |
                                             QDialogButtonBox::Cancel |
                                             QDialogButtonBox::Reset );

    m_filter_line = new QLineEdit();
        m_filter_line->setPlaceholderText(tr("Find plugin"));
        m_filter_line->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

    m_view = new PluginTreeView();

    m_detail_btn = new QPushButton();
        m_detail_btn->setIcon(QIcon(":/icons/info.svg"));
        m_detail_btn->setIconSize(QSize(22,22));
        m_detail_btn->setFixedSize(32, 32);

    auto m_layout = new QGridLayout();
        m_layout->addWidget(m_filter_line, 0, 0, 1, 1);
        m_layout->addWidget(m_detail_btn, 0, 1, 1, 1);
        m_layout->addWidget(m_view, 1, 0, 1, 2);
        m_layout->addWidget(m_dialog_buttons, 2, 0, 1, 2);

    setLayout(m_layout);
}

void PluginManagerDialog::connectSignals()
{
    connect(m_filter_line, &QLineEdit::textChanged, m_view, &PluginTreeView::setFilterFixedString);
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &PluginManagerDialog::onDialogClicked);
}


void PluginManagerDialog::onDialogClicked(QAbstractButton* aBtn)
{
    switch( m_dialog_buttons->standardButton( aBtn ) )
    {
        case QDialogButtonBox::Apply:
            PluginManager::instance().saveState();
            break;

        case QDialogButtonBox::Ok:
            PluginManager::instance().saveState();
            hide();
            break;

        case QDialogButtonBox::Reset:
            PluginManager::instance().resetState();
            m_view->expandAll();
            break;

        case QDialogButtonBox::Cancel:
            PluginManager::instance().restoreState();
            hide();
            break;

        default:
            break;
    }
}
