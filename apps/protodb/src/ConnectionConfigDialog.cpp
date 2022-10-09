#include "ConnectionConfigDialog.h"
#include "LogFormatWidget.h"

#include <protodb/IOWidget.h>
#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/creators/IOWidgetCreatorInterface.h>

#include <protodb/factories/GlobalFactoryStorage.h>

#include <QFrame>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QAction>

ConnectionConfigDialog::ConnectionConfigDialog(QWidget* aParent)
    : QDialog(aParent)
{
    createGui();
    connectSignals();
}

void ConnectionConfigDialog::getConfig(nlohmann::json& json)
{
    nlohmann::json io_device_cfg = nlohmann::json::object();
    auto io_widget = dynamic_cast<IOWidget*>(m_current_iowiget);
    if (io_widget) {
        io_widget->config(io_device_cfg);
    }
    else {
        io_device_cfg["CID"] = "NULL";
    }

    json["IODevice"] = io_device_cfg;
}

void ConnectionConfigDialog::createGui()
{
    // --------[CREATE DUMMY WIDGET]--------- //
    m_dummy_wgt = new QWidget();
    {
        auto label = new QLabel(tr("Please, load plugins"));
        auto laout = new QVBoxLayout();
            laout->addWidget(label);
            laout->setAlignment(label, Qt::AlignCenter);
        m_dummy_wgt->setLayout(laout);
    }

    // --------[CREATE IO WIDGETS]--------- //
    auto factory = IOWidgetFactory::globalInstance();
    if (!factory) {
        GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory);
        factory = IOWidgetFactory::globalInstance();
    }

    auto creators = factory->getAllCreators();
    for(auto &it: creators) {
        if (it) {
            auto wgt = it.data()->create();

            m_io_widgets[it->cid()] = wgt;
            m_selection_btns[it->cid()] = new QRadioButton(wgt->name());
        }
    }

    // --------[BUTTONS]--------- //
    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok |
                                         QDialogButtonBox::Apply |
                                         QDialogButtonBox::Cancel |
                                         QDialogButtonBox::Reset |
                                         QDialogButtonBox::RestoreDefaults);

    m_selection_group = new QButtonGroup();
    for (auto &it: m_selection_btns) {
        m_selection_group->addButton(it);
    }

    if (!m_selection_btns.empty()) {
        m_selection_btns.first()->setChecked(true);
    }

    // --------[LOG FILE]--------- //
    m_log_btn = new QPushButton();
        m_log_btn->setIcon(QIcon(":/icons/folder.svg"));
        m_log_btn->setToolTip(tr("Show File Dialog"));

    m_log_le = new QLineEdit();
        m_log_le->setPlaceholderText(tr("Path to log file"));

    // --------[SCRIPT FILE]--------- //
    m_scr_btn = new QPushButton();
        m_scr_btn->setIcon(QIcon(":/icons/folder.svg"));
        m_scr_btn->setToolTip(tr("Show File Dialog"));

    m_scr_le = new QLineEdit();
        m_scr_le->setPlaceholderText(tr("Path to script file"));

    // ------[CONFIG WIDGETS] ------ //
    m_current_iowiget  = m_io_widgets.empty() ? m_dummy_wgt : m_io_widgets.first();
    m_log_format_wiget = new LogFormatWidget();

    // ---------[SELECTOR FRAME]---------//
    auto selector_frame = new QFrame();
        selector_frame->setFrameShape(QFrame::StyledPanel);
        selector_frame->setFrameShadow(QFrame::Raised);

    m_selector_layout = new QVBoxLayout();
        m_selector_layout->setAlignment(Qt::AlignTop);
        m_selector_layout->addWidget(new QLabel(tr("Select mode:")));

        for (auto &btn: m_selection_btns) {
            m_selector_layout->addWidget(btn);
        }


    selector_frame->setLayout(m_selector_layout);

    m_layout = new QGridLayout();
        m_layout->addWidget(selector_frame, 0 , 0, 4, 1);
        m_layout->addWidget(m_scr_le, 0 , 1, 1, 1);
        m_layout->addWidget(m_scr_btn, 0 , 2, 1, 1);
        m_layout->addWidget(m_current_iowiget, 1 , 1, 1, 2);
        m_layout->addWidget(m_log_le, 2, 1, 1, 1);
        m_layout->addWidget(m_log_btn, 2 , 2, 1, 1);
        m_layout->addWidget(m_log_format_wiget, 3 ,1, 1, 2);
        m_layout->addWidget(m_dialog_btn, 4, 0, 1, 3);
        m_layout->setColumnStretch(0, 0);
        m_layout->setColumnStretch(1, 1);
        m_layout->setColumnStretch(2, 0);
        m_layout->setRowStretch(0, 0);
        m_layout->setRowStretch(1, 1);
        m_layout->setRowStretch(2, 0);
        m_layout->setRowStretch(3, 1);
        m_layout->setRowStretch(4, 0);

    setLayout(m_layout);
}

void ConnectionConfigDialog::connectSignals()
{
    auto factory = GlobalFactoryStorage::getFactory(IOWidgetFactory::fid());
    connect(factory, &FactoryAbstract::sCreatorAdded, this, [this](QString cid) {
        auto factory = qobject_cast<IOWidgetFactory*>(QObject::sender());

        if (factory) {
            auto wgt = factory->createIOWidget(cid);
            auto btn = new QRadioButton(wgt->name());

            m_io_widgets[ cid ] = wgt;
            m_selection_btns[ cid ] = btn;

            m_selection_group->addButton(btn);
            m_selector_layout->addWidget(btn);
        }
    });

    connect(factory, &FactoryAbstract::sCreatorRemoved, this, [this](QString cid) {
        auto factory = qobject_cast<IOWidgetFactory*>(QObject::sender());

        if (factory) {
            auto wgt = m_io_widgets.take(cid);
            auto btn = m_selection_btns.take(cid);

            if (wgt == m_current_iowiget) {
                if (!m_io_widgets.empty()) {
                    auto cid = m_io_widgets.firstKey();

                    m_selection_btns[ cid ]->setChecked(true);
                    replaceIOWiget(m_io_widgets.firstKey());
                }
            }

            m_selector_layout->removeWidget(btn);
            m_selection_group->removeButton(btn);

            delete wgt;
            delete btn;
        }
    });

    connect(m_dialog_btn, &QDialogButtonBox::clicked, this, [this](QAbstractButton* btn){
        switch( m_dialog_btn->standardButton( btn ) )
        {
            case QDialogButtonBox::Apply:
                break;

            case QDialogButtonBox::Ok:
                hide();
                break;

            case QDialogButtonBox::Reset:
                break;

            case QDialogButtonBox::Cancel:
                hide();
                break;

            default:
                break;
        }
    });

    connect(m_selection_group,
            QOverload<QAbstractButton *>::of(&QButtonGroup::buttonReleased), this, [this](QAbstractButton* btn)
    {
        auto cid  = m_selection_btns.key(btn, QString());
        replaceIOWiget(cid);
    });

    connect(m_log_btn, &QPushButton::released, this, [this]() {
        QString path = m_log_le->text();
        showFileDialog(path);

        m_log_le->setText(path);
    });

    connect(m_scr_btn, &QPushButton::released, this, [this]() {
        QString path = m_scr_le->text();
        showFileDialog(path);

        m_scr_le->setText(path);
    });

    connect(m_scr_le, &QLineEdit::textChanged, this, [](const QString& text) {

    });
}

void ConnectionConfigDialog::replaceIOWiget(const QString& cid)
{
    auto wgt = cid.isEmpty() ? m_dummy_wgt : m_io_widgets.value(cid, nullptr);

    if (wgt) {
        auto item = m_layout->replaceWidget(m_current_iowiget, wgt);
        if (item) {
            m_current_iowiget->hide();
                wgt->show();
            m_current_iowiget = wgt;

            delete item;
        }
    }
}

void ConnectionConfigDialog::showFileDialog(QString& path)
{
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::List);

    QStringList fileNames;
    if (fileDialog.exec()) {
      fileNames = fileDialog.selectedFiles();

      if (fileNames.size() != 0) {
            path = fileNames.back();
      }
    }
}
