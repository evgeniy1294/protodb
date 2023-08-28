#include "protodb/plugins/PluginTreeView.h"
#include "protodb/plugins/PluginManagerDialog.h"

#include <protodb/plugins/PluginManager.h>

#include <QApplication>
#include <QDir>
#include <QDesktopServices>
#include <QAction>
#include <QDataWidgetMapper>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QGridLayout>
#include <QSortFilterProxyModel>
#include <QAbstractButton>
#include <QTextBrowser>
#include <QLabel>
#include <QPlainTextEdit>

using namespace protodb;

PluginManagerDialog::PluginManagerDialog(QWidget* parent)
    : QDialog(parent)
    , m_mapper(new QDataWidgetMapper(this))
{
    createGui();
    connectSignals();

    initMapper();

    resize(940, 520);
    setWindowTitle(tr("Plugins"));
}

void PluginManagerDialog::initMapper()
{
    m_mapper->setModel(m_view->model());
    m_mapper->addMapping(m_plugin_location, PluginManager::kColFile);
    m_mapper->addMapping(m_plugin_descr, PluginManager::kColDescription);
    m_mapper->addMapping(m_plugin_deps, PluginManager::kColRelations);
}

void PluginManagerDialog::createGui()
{
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok |
                                             QDialogButtonBox::Apply |
                                             QDialogButtonBox::Cancel |
                                             QDialogButtonBox::Reset );

    m_plugin_location = new QLineEdit();
        m_plugin_location->setPlaceholderText(tr("Location"));
        m_plugin_location->setReadOnly(true);
        //m_plugin_location->hide();

    m_plugin_descr = new QTextBrowser();
        m_plugin_descr->setPlaceholderText(tr("Description"));
        //m_plugin_descr->hide();

    m_plugin_deps = new QPlainTextEdit();
        m_plugin_deps->setReadOnly(true);
        m_plugin_deps->setPlaceholderText(tr("Dependencies"));
        //m_plugin_deps->hide();

    m_filter_line = new QLineEdit();
        m_filter_line->setPlaceholderText(tr("Find plugin"));
        m_filter_line->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);
        m_filter_line->setMinimumHeight(28);

    m_view = new PluginTreeView();

   /* m_detail_btn = new QPushButton();
        m_detail_btn->setIcon(QIcon(":/icons/arrow_right.svg"));
        m_detail_btn->setIconSize(QSize(18,18));
        m_detail_btn->setFixedSize(28, 28);*/

    auto m_top_layout = new QHBoxLayout();
        m_top_layout->addWidget(m_filter_line);
        //m_top_layout->addWidget(m_detail_btn);

    auto detail_layout = new QVBoxLayout();
        detail_layout->addWidget(m_plugin_location);
        detail_layout->addWidget(m_plugin_deps);
        detail_layout->addWidget(m_plugin_descr);

    auto base_layout = new QVBoxLayout();
        base_layout->addWidget(m_view);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(m_dialog_buttons);

    auto m_main_layout = new QGridLayout();
        m_main_layout->addLayout(m_top_layout, 0, 0, 1, 2);
        m_main_layout->addLayout(base_layout, 1, 0, 1, 1);
        m_main_layout->addLayout(detail_layout, 1, 1, 1, 1);
        m_main_layout->addLayout(bottom_layout, 2, 0, 1, 2);
        m_main_layout->setColumnStretch(0, 5);
        m_main_layout->setColumnStretch(1, 3);

    setLayout(m_main_layout);
}

void PluginManagerDialog::connectSignals()
{
    auto copy = new QAction();
        copy->setIcon(QIcon(":/icons/folder.svg"));
    m_plugin_location->addAction(copy, QLineEdit::TrailingPosition);

    connect(copy, &QAction::triggered, this, [this](bool checked) {
        auto dir = QFileInfo( m_plugin_location->text() ).absoluteDir();
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absolutePath()));
    });

    connect(m_view, &QTreeView::clicked, this, [this](const QModelIndex& index) {
        if (!m_mapper->model()->hasChildren(index)) {
            m_mapper->setRootIndex(index.parent());
            m_mapper->setCurrentModelIndex(index);
        }
    });

    connect(m_filter_line, &QLineEdit::textChanged, m_view, &PluginTreeView::setFilterFixedString);
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &PluginManagerDialog::onDialogClicked);
/*
    connect(m_detail_btn, &QPushButton::released, this, [this]() {
        if (m_plugin_location->isHidden()) {
            m_plugin_location->show();
            m_plugin_deps->show();
            m_plugin_descr->show();
            m_detail_btn->setIcon(QIcon(":/icons/arrow_left.svg"));
        }
        else {
            m_plugin_location->hide();
            m_plugin_deps->hide();
            m_plugin_descr->hide();
            m_detail_btn->setIcon(QIcon(":/icons/arrow_right_detail.svg"));
        }
    });
*/
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
