#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QDataWidgetMapper>
#include <QEvent>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

#include <protodb/PluginManager.h>
#include <protodb/PluginManagerDialog.h>
#include <protodb/PluginDetailDialog.h>
#include <protodb/PluginTreeView.h>

PluginTreeView::PluginTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_pm = &PluginManager::instance();
    m_fm = new QSortFilterProxyModel();
        m_fm->setSourceModel(m_pm);
        m_fm->setRecursiveFilteringEnabled(true);
        m_fm->setFilterKeyColumn(PluginManager::kColName);
        m_fm->setFilterCaseSensitivity(Qt::CaseInsensitive);
    QTreeView::setModel(m_fm);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);


    QHeaderView* hh = header();
        hh->setStretchLastSection(false);
        hh->setDefaultAlignment(Qt::AlignCenter);
        hh->setSectionResizeMode(QHeaderView::Fixed);
        hh->setSectionResizeMode(PluginManager::kColName, QHeaderView::Stretch);
        hh->setSectionResizeMode(PluginManager::kColEnabled, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(PluginManager::kColVersion, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(PluginManager::kColVendor, QHeaderView::ResizeToContents);
        hh->hideSection(PluginManager::kColIid);
        hh->hideSection(PluginManager::kColGroup);
        hh->hideSection(PluginManager::kColFile);
        hh->hideSection(PluginManager::kColDescription);

    m_mapper = new QDataWidgetMapper(this);
        m_mapper->setModel(m_pm);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_detail_dialog = new PluginDetailDialog(this);
        m_detail_dialog->resize(540, 480);
        m_detail_dialog->setMapper(m_mapper);

    expandAll();
    connectSignals();
}

void PluginTreeView::setModel(QAbstractItemModel* model)
{
    return;
}

void PluginTreeView::setFilterFixedString(const QString& pattern)
{
    m_fm->setFilterFixedString(std::move(pattern));
}


bool PluginTreeView::edit(const QModelIndex& index, EditTrigger trigger, QEvent* event)
{
    bool ret = QTreeView::edit(index, trigger, event);

    if (!ret) {
        auto err = m_pm->lastError();

        if (!err.isEmpty()) {
            QMessageBox msgbox;
            {
                msgbox.setText( tr("Warning") );
                msgbox.setInformativeText( err );
                msgbox.setStandardButtons(QMessageBox::Ok);
                msgbox.setDefaultButton(QMessageBox::Ok);
                msgbox.setIcon(QMessageBox::Icon::Warning);

                QSpacerItem* horizontalSpacer = new QSpacerItem(540, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
                QGridLayout* layout = (QGridLayout*)msgbox.layout();
                layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
            }
            msgbox.exec();

            m_pm->clearError();
        }
    }
    return ret;
}

void PluginTreeView::createMenu()
{

}

void PluginTreeView::createActions()
{

}

void PluginTreeView::connectSignals()
{
    connect(this, &PluginTreeView::doubleClicked, this, [this](const QModelIndex &a_index) {
        auto index = m_fm->mapToSource(a_index);

        if (!m_pm->hasChildren(index) && index.column() == PluginManager::kColName) {
            m_mapper->setRootIndex(index.parent());
            m_mapper->setCurrentModelIndex(index);

            m_detail_dialog->show();
        }
    });
}
