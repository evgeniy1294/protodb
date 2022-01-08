#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTextBrowser>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QAbstractButton>

#include <protodb/PluginManager.h>
#include <protodb/PluginDetailDialog.h>


PluginDetailDialog::PluginDetailDialog(QWidget* parent)
    : QDialog(parent)
    , m_mapper(nullptr)
{
    createGui();
    connectSignals();

    setWindowTitle("Plugin Details");
}

void PluginDetailDialog::createGui()
{
    m_btn_box = new QDialogButtonBox(QDialogButtonBox::Ok);

    m_name = new QLineEdit();
        m_name->setReadOnly(true);
    m_version = new QLineEdit();
        m_version->setReadOnly(true);
    m_vendor = new QLineEdit();
        m_vendor->setReadOnly(true);
    m_group = new QLineEdit();
        m_group->setReadOnly(true);
    m_location = new QLineEdit();
        m_location->setReadOnly(true);
    m_descr = new QTextBrowser();
        m_descr->setReadOnly(true);
        m_descr->setToolTip(tr("Description"));
    m_deps = new QListWidget();
       // m_deps->insertItem(0, deps[0]);
      //  m_deps->insertItem(1, deps[1]);
        m_deps->setToolTip(tr("Dependencies"));

    auto m_layout = new QGridLayout();
        m_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        m_layout->addWidget(new QLabel(tr("Name")), 0, 0, 1, 1);
        m_layout->addWidget(m_name, 0, 1, 1, 1);
        m_layout->addWidget(m_deps, 0, 2, 4, 1);
        m_layout->addWidget(new QLabel(tr("Version")), 1, 0, 1, 1);
        m_layout->addWidget(m_version, 1, 1, 1, 1);
        m_layout->addWidget(new QLabel(tr("Vendor")), 2, 0, 1, 1);
        m_layout->addWidget(m_vendor, 2, 1, 1, 1);
        m_layout->addWidget(new QLabel(tr("Group")), 3, 0, 1, 1);
        m_layout->addWidget(m_group, 3, 1, 1, 1);
        m_layout->addWidget(new QLabel(tr("Location")), 4, 0, 1, 1);
        m_layout->addWidget(m_location, 4, 1, 1, 2);
        m_layout->addWidget(m_descr, 5, 0, 1, 3);

        m_layout->addWidget(m_btn_box, 6, 0, 1, 3);

    setLayout(m_layout);
}

void PluginDetailDialog::connectSignals()
{
    connect(m_btn_box, &QDialogButtonBox::clicked, this, &PluginDetailDialog::onDialogClicked);
}

QDataWidgetMapper* PluginDetailDialog::mapper() const
{
    return m_mapper;
}

void PluginDetailDialog::setMapper(QDataWidgetMapper* mapper)
{
    if (m_mapper != mapper) {
        if ( m_mapper != nullptr ) {
            m_mapper->clearMapping();
            disconnect(m_mapper);
        }

        m_mapper = mapper;
        m_mapper->addMapping(m_name, PluginManager::kColumnName);
        m_mapper->addMapping(m_version, PluginManager::kColumnVersion);
        m_mapper->addMapping(m_vendor, PluginManager::kColumnVendor);
        m_mapper->addMapping(m_group, PluginManager::kColumnGroup);
        m_mapper->addMapping(m_location, PluginManager::kColumnLocation);
        m_mapper->addMapping(m_descr, PluginManager::kColumnDescription);

        auto onIndexChanged = [this](int idx) {
            auto index = m_mapper->model()->index(idx, PluginManager::kColumnDependencies, m_mapper->rootIndex());
            auto deps = m_mapper->model()->data(index, Qt::DisplayRole).toStringList();

            m_deps->clear();

            for (auto& d: deps) {
                m_deps->addItem(d);
            }
        };

        connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, this, onIndexChanged);
        onIndexChanged(m_mapper->currentIndex());
    }
}

void PluginDetailDialog::onDialogClicked(QAbstractButton* btn)
{
    switch( m_btn_box->standardButton(btn) )
    {
        case QDialogButtonBox::Ok:
            hide();
            break;

        default:
            break;
    }
}
