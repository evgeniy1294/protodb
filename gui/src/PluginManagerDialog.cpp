#include <protodb/PluginManager.h>
#include <protodb/PluginTreeView.h>
#include <protodb/PluginManagerDialog.h>

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
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

    m_view = new PluginTreeView();

    auto m_layout = new QVBoxLayout();
        m_layout->addWidget(m_filter_line);
        m_layout->addWidget(m_view);
        m_layout->addWidget(m_dialog_buttons);

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
