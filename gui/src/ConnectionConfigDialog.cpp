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

#include <protodb/ConnectionConfigDialog.h>
//#include "SerialIOWidget.h"
//#include "NetIOWidget.h"
#include "protodb/LogFormatWidget.h"

ConnectionConfigDialog::ConnectionConfigDialog(QWidget* aParent)
    : QDialog(aParent)
{
    createGui();
    connectSignals();
}

void ConnectionConfigDialog::createGui()
{
    // --------[BUTTONS]--------- //
    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok |
                                         QDialogButtonBox::Apply |
                                         QDialogButtonBox::Cancel |
                                         QDialogButtonBox::Reset |
                                         QDialogButtonBox::RestoreDefaults);

    auto serial_radio  = new QRadioButton(tr("Serial"));
        serial_radio->setChecked(true);
    auto network_radio = new QRadioButton(tr("Network"));
    auto bridge_radio  = new QRadioButton(tr("Bridge"));

    m_mode_group = new QButtonGroup();
        m_mode_group->addButton(serial_radio, 0);
        m_mode_group->addButton(network_radio, 0);
        m_mode_group->addButton(bridge_radio, 0);

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
    auto serial_config_widget = new QWidget();//NetIOWidget();
        m_widgets.append(serial_config_widget);

    auto log_config_frame = new LogFormatWidget();

    // ---------[SELECTOR FRAME]---------//
    auto selector_frame = new QFrame();
        selector_frame->setFrameShape(QFrame::StyledPanel);
        selector_frame->setFrameShadow(QFrame::Raised);

    auto selector_layout = new QVBoxLayout();
        selector_layout->setAlignment(Qt::AlignTop);
        selector_layout->addWidget(new QLabel(tr("Select mode:")));
        selector_layout->addWidget(serial_radio);
        selector_layout->addWidget(network_radio);
        selector_layout->addWidget(bridge_radio);

    selector_frame->setLayout(selector_layout);

    auto main_layout = new QGridLayout();
        main_layout->addWidget(selector_frame, 0 , 0, 4, 1);
        main_layout->addWidget(m_scr_le, 0 , 1, 1, 1);
        main_layout->addWidget(m_scr_btn, 0 , 2, 1, 1);
        main_layout->addWidget(serial_config_widget, 1 ,1, 1, 2);
        main_layout->addWidget(m_log_le, 2, 1, 1, 1);
        main_layout->addWidget(m_log_btn, 2 , 2, 1, 1);
        main_layout->addWidget(log_config_frame, 3 ,1, 1, 2);
        main_layout->addWidget(m_dialog_btn, 4, 0, 1, 3);
        main_layout->setColumnStretch(0, 0);
        main_layout->setColumnStretch(1, 1);
        main_layout->setColumnStretch(2, 0);
        main_layout->setRowStretch(0, 0);
        main_layout->setRowStretch(1, 1);
        main_layout->setRowStretch(2, 0);
        main_layout->setRowStretch(3, 1);
        main_layout->setRowStretch(4, 0);

    setLayout(main_layout);
}

void ConnectionConfigDialog::connectSignals()
{
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
