#include "ProtodbConfigDialog.h"
#include "ProtodbConfigStorage.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QEvent>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>

using namespace protodb;

ProtodbConfigDialog::ProtodbConfigDialog(QObject* parent)
{
    create_gui();
    connect_signals();

    resize(600, 220);

    nlohmann::json cfg; ProtodbConfigStorage::instance().config(cfg);
    setState(cfg);
}

void ProtodbConfigDialog::setState(const nlohmann::json& json)
{
    if (json.is_object()) {
        auto sessionLocation = json.value("SessionsLocation", ProtodbConfigStorage::instance().sessionsLocation());
        m_sessions_storage_path->setText(sessionLocation);

        auto userPluginsLocation = json.value("UserPluginsLocation", ProtodbConfigStorage::instance().userPluginsLocation());
        m_plugin_install_path->setText(userPluginsLocation);
    }
}

void ProtodbConfigDialog::state(nlohmann::json& json)
{
    json = nlohmann::json::object();
        json["UserPluginsLocation"] = m_plugin_install_path->text();
        json["SessionsLocation"]    = m_sessions_storage_path->text();
}

void ProtodbConfigDialog::create_gui()
{
    setWindowTitle(tr("Application options"));

    m_plugin_install_path = new QLineEdit();

    m_sessions_storage_path = new QLineEdit();

    m_plugin_install_fd = new QPushButton();
        m_plugin_install_fd->setIcon(QIcon(":/icons/folder.svg"));
        m_plugin_install_fd->setToolTip(tr("Show File Dialog"));

    m_sessions_storage_fd = new QPushButton();
        m_sessions_storage_fd->setIcon(QIcon(":/icons/folder.svg"));
        m_sessions_storage_fd->setToolTip(tr("Show File Dialog"));

    m_dialog_btns = new QDialogButtonBox( QDialogButtonBox::Ok |
                                          QDialogButtonBox::Apply |
                                          QDialogButtonBox::Cancel |
                                          QDialogButtonBox::Reset |
                                          QDialogButtonBox::RestoreDefaults);

    auto main_layout = new QGridLayout();
        main_layout->addWidget(new QLabel(tr("Sessions folder:")), 0, 0, 1, 2, Qt::AlignLeft);
        main_layout->addWidget(m_sessions_storage_path, 1, 0);
        main_layout->addWidget(m_sessions_storage_fd, 1, 1);
        main_layout->addWidget(new QLabel(tr("Plugins manually install path:")), 2, 0, 1, 2, Qt::AlignLeft);
        main_layout->addWidget(m_plugin_install_path, 3, 0);
        main_layout->addWidget(m_plugin_install_fd, 3, 1);
        main_layout->addWidget(m_dialog_btns, 4, 0, 1, 2, Qt::AlignBottom);
        main_layout->setRowStretch(4, 1);

    setLayout(main_layout);
}

void ProtodbConfigDialog::connect_signals()
{
    connect(m_plugin_install_fd, &QPushButton::released, this, [this]() {
        auto path = QFileDialog::getExistingDirectory(nullptr, QString(), m_plugin_install_path->text());
        if (!path.isEmpty())
            m_plugin_install_path->setText(path);
    });

    connect(m_sessions_storage_fd, &QPushButton::released, this, [this]() {
        auto path = QFileDialog::getExistingDirectory(nullptr, QString(), m_sessions_storage_path->text());
        if (!path.isEmpty())
            m_sessions_storage_path->setText(path);
    });

    connect(m_dialog_btns, &QDialogButtonBox::clicked, this, [this](QAbstractButton* btn){
        switch( m_dialog_btns->standardButton( btn ) )
        {
            case QDialogButtonBox::Ok: {
                nlohmann::json json; state(json);
                ProtodbConfigStorage::instance().setConfig(json);
                hide();
            } break;

            case QDialogButtonBox::Apply: {
                nlohmann::json json; state(json);
                ProtodbConfigStorage::instance().setConfig(json);
            } break;

            case QDialogButtonBox::RestoreDefaults: {
                QMessageBox msgbox;
                {
                    msgbox.setText( tr("Restore default configs?") );
                    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgbox.setDefaultButton(QMessageBox::No);
                    msgbox.setIcon(QMessageBox::Icon::Warning);

                        msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
                        msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
                }

                if (msgbox.exec() == QMessageBox::Yes) {
                    ProtodbConfigStorage::instance().setDefaultConfig();
                    nlohmann::json cfg; ProtodbConfigStorage::instance().config(cfg);

                    setState(cfg);
                }
            } break;

            case QDialogButtonBox::Reset: {
                nlohmann::json cfg; ProtodbConfigStorage::instance().config(cfg);
                setState(cfg);
            } break;

            case QDialogButtonBox::Cancel: {
                nlohmann::json cfg; ProtodbConfigStorage::instance().config(cfg);
                setState(cfg);
                hide();
            } break;

            default:
                break;
        }
    });
}

bool ProtodbConfigDialog::event(QEvent* e)
{
    if (e->type() == QEvent::Hide) {
        nlohmann::json cfg; ProtodbConfigStorage::instance().config(cfg);
        setState(cfg);

        return true;
    }

    return QWidget::event(e);
}
