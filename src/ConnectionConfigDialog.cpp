#include <QFrame>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QAction>

#include "ConnectionConfigDialog.h"
#include "SerialConfigWidget.h"
#include "log_config_widget.h"

ConnectionConfigDialog::ConnectionConfigDialog(QWidget* aParent)
    : QDialog(aParent)
{
    createGui();
    connectSignals();
}

void ConnectionConfigDialog::createGui()
{
    // --------[BUTTONS]--------- //
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel );

    auto serial_radio  = new QRadioButton(tr("Serial"));
        serial_radio->setChecked(true);
    auto network_radio = new QRadioButton(tr("Network"));
    auto bridge_radio  = new QRadioButton(tr("Bridge"));

    m_mode_selector = new QButtonGroup();
        m_mode_selector->addButton(serial_radio, 0);
        m_mode_selector->addButton(network_radio, 0);
        m_mode_selector->addButton(bridge_radio, 0);

    // --------[LOG FILE]--------- //
    m_file_button = new QPushButton();
        m_file_button->setIcon(QIcon(":/icons/file_search.svg"));
        m_file_button->setToolTip(tr("Show File Dialog"));

    m_logfile = new QLineEdit();
        m_logfile->setPlaceholderText(tr("Path to log file"));

    // ------[CONFIG WIDGETS] ------ //
    auto serial_config_widget = new SerialConfigWidget();
        m_widgets.append(serial_config_widget);

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

    // ---------[LAYOUT]---------- //

    auto layout = new QGridLayout();
        layout->addWidget(selector_frame, 0 , 0, 2, 1);
        layout->addWidget(m_file_button, 0 , 2, 1, 1);
        layout->addWidget(m_logfile, 0, 1, 1, 1);
        layout->addWidget(serial_config_widget, 1 ,1, 1, 2);
        layout->addWidget(m_dialog_buttons, 2, 0, 1, 3);
        layout->setColumnStretch(0, 0);
        layout->setColumnStretch(1, 1);
        layout->setColumnStretch(2, 0);

        setLayout(layout);
}

void ConnectionConfigDialog::connectSignals()
{
    connect(m_file_button, &QPushButton::released, this, [this]() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        QStringList fileNames;
        if (fileDialog.exec()) {
          fileNames = fileDialog.selectedFiles();

          if (fileNames.size() != 0) {
                m_logfile->setText(fileNames.back());
          }
        }

    });
}

