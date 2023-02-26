#include "SeanseConfigDialog.h"
#include "LogFormatWidget.h"
#include "DelimetersConfigWidget.h"
#include "MainClass.h"

#include <protodb/IOWidget.h>
#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/creators/IOWidgetCreatorInterface.h>

#include <protodb/factories/GlobalFactoryStorage.h>

#include <QFrame>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QTabWidget>

using namespace protodb;

SeanceConfigDialog::SeanceConfigDialog(QWidget* aParent)
    : QDialog(aParent)
    , m_curr_cfg(nlohmann::json::object())
{
    createGui();
    connectSignals();

    setDefaultState();
}

void SeanceConfigDialog::createGui()
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

    if (!m_io_widgets.empty()) {
        m_curr_cid = m_io_widgets.firstKey();
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
    m_tab_widget = new QTabWidget;

    m_current_iowiget  = m_io_widgets.empty() ? m_dummy_wgt : m_io_widgets.first();
    m_log_format_wiget = new LogFormatWidget();
        m_tab_widget->addTab(m_log_format_wiget, tr("Log"));

    m_delimeters_cfg_widget = new DelimetersConfigWidget();
        m_tab_widget->addTab(m_delimeters_cfg_widget, tr("Delimeters"));

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
        m_layout->addWidget(m_tab_widget, 3 ,1, 1, 2);
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
    setWindowTitle("Seance config");
}

void SeanceConfigDialog::connectSignals()
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

            wgt->deleteLater();
            btn->deleteLater();
        }
    });

    connect(m_dialog_btn, &QDialogButtonBox::clicked, this, [this](QAbstractButton* btn){
        switch( m_dialog_btn->standardButton( btn ) )
        {
            case QDialogButtonBox::Apply: {
                state(m_curr_cfg);
                emit accepted();
            } break;

            case QDialogButtonBox::Ok:
                state(m_curr_cfg);
                hide();
                emit accepted();
                break;

            case QDialogButtonBox::RestoreDefaults: {
                QMessageBox msgbox;
                {
                    msgbox.setText( tr("Restore default configs?") );
                    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgbox.setDefaultButton(QMessageBox::No);
                    msgbox.setIcon(QMessageBox::Icon::Warning);
                }

                if (msgbox.exec() == QMessageBox::Yes) {
                    setDefaultState();
                    state(m_curr_cfg);
                    emit accepted();
                }

            } break;

            case QDialogButtonBox::Reset:
                setState(m_curr_cfg);
                break;

            case QDialogButtonBox::Cancel:
                setState(m_curr_cfg);
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

void SeanceConfigDialog::replaceIOWiget(const QString& cid)
{
    auto wgt = cid.isEmpty() ? m_dummy_wgt : m_io_widgets.value(cid, nullptr);

    if (wgt) {
        auto item = m_layout->replaceWidget(m_current_iowiget, wgt);
        if (item) {
            m_curr_cid = cid;

            m_current_iowiget->hide();
                wgt->show();
            m_current_iowiget = wgt;

            delete item;
        }
    }
}

void SeanceConfigDialog::showFileDialog(QString& path)
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

bool SeanceConfigDialog::event(QEvent *e)
{
    if (e->type() == QEvent::Show) {
        nlohmann::json cfg; MainClass::instance().seanceConfigs(cfg);
        setState(cfg);

        return true;
    }

    return QWidget::event(e);
}

// Должна быть метка активного элемента
void SeanceConfigDialog::setConfig(const nlohmann::json& json)
{

}

void SeanceConfigDialog::config(nlohmann::json& json) const
{        

}

void SeanceConfigDialog::defaultConfig(nlohmann::json& json) const
{

}

void SeanceConfigDialog::setState(const nlohmann::json& json)
{
    if (!json.is_object()) {
        return;
    }

    auto log_configs = json.value("LogConfigs", nlohmann::json::object());
        m_log_format_wiget->setConfig(log_configs);

    auto delim_configs = json.value("Delimeters", nlohmann::json::object_t());
        m_delimeters_cfg_widget->setConfig(delim_configs);

    QString log_file_path = json.value("LogFile", QString());
        m_log_le->setText(log_file_path);

    QString script_file_path = json.value("ScriptFile", QString());
        m_scr_le->setText(script_file_path);

    auto configs  = json.value("Configs",  nlohmann::json::object());
    for (auto& [cid, cfg] : configs.items()) {
        auto gcid = cfg.value("gcid", QString("Null"));

        if (m_io_widgets.contains(gcid)) {
            m_io_widgets[gcid]->setConfig(cfg);
        }
    }

    auto selectedCID = json.value("SelectedCID", std::string("Null"));
    auto deviceCfg = configs.value(selectedCID, nlohmann::json::object());
    auto gcid = deviceCfg.value("gcid", QString("Null"));

    if (m_io_widgets.contains(gcid)) {
        auto buttons = m_selection_group->buttons();

        for (int i = 0; i < buttons.size(); ++i) {
            auto btn = buttons[i];
            if ( m_selection_btns.key(btn, QString()) == gcid ) {
                btn->setChecked(true);
                replaceIOWiget(gcid);
            }
        }
    }

    m_curr_cfg.clear();
    state(m_curr_cfg);

    return;
}

void SeanceConfigDialog::state(nlohmann::json& json) const
{
    json = nlohmann::json::object();

    auto log_configs = nlohmann::json::object();
        m_log_format_wiget->config(log_configs);

    auto delim_configs = nlohmann::json::object();
        m_delimeters_cfg_widget->config(delim_configs);

    auto configs = nlohmann::json::object();
    for (auto it = m_io_widgets.begin(); it != m_io_widgets.end(); ++it) {
        nlohmann::json cfg = nlohmann::json::object();
            it.value()->config(cfg);

        auto deviceCID = it.value()->deviceCID().toStdString();
        configs[deviceCID] = cfg;
    }

    if (m_io_widgets.contains(m_curr_cid)) {
        json["SelectedCID"] = m_io_widgets[m_curr_cid]->deviceCID().toStdString();
    }
    else {
        json["SelectedCID"] = "Null";
    }

    json["Configs"]     = configs;
    json["LogConfigs"]  = log_configs;
    json["Delimeters"]  = delim_configs;
    json["ScriptFile"]  = m_scr_le->text();
    json["LogFile"]     = m_log_le->text();

    return;
}

void SeanceConfigDialog::defaultState(nlohmann::json& json) const
{
    json = nlohmann::json::object();

    auto log_configs = nlohmann::json::object();
        m_log_format_wiget->defaultConfig(log_configs);

    auto delim_configs = nlohmann::json::object();
        m_delimeters_cfg_widget->defaultConfig(delim_configs);

    auto configs = nlohmann::json::object();
    for (auto it = m_io_widgets.begin(); it != m_io_widgets.end(); ++it) {
        nlohmann::json cfg = nlohmann::json::object();
            it.value()->defaultConfig(cfg);

        auto deviceCID = it.value()->deviceCID().toStdString();
        configs[deviceCID] = cfg;
    }

    if (m_io_widgets.contains(m_curr_cid)) {
        json["SelectedCID"] = m_io_widgets[m_curr_cid]->deviceCID().toStdString();
    }
    else {
        json["SelectedCID"] = "Null";
    }

    json["Configs"]     = configs;
    json["LogConfigs"]  = log_configs;
    json["Delimeters"]  = delim_configs;
    json["ScriptFile"]  = "";
    json["LogFile"]     = "";

    return;
}
