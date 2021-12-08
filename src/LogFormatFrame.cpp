#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <qnlohmann.h>
#include "LogFormatFrame.h"
#include "LogFormatter.h"

LogFormatFrame::LogFormatFrame(QWidget* aParent)
    : ConfigFrame(aParent)
{
    createGui();
    connectSignals();
}

void LogFormatFrame::defaultConfig(nlohmann::json &json) const
{
    LogFormatter::formatterDefaultConfig(json);
}

void LogFormatFrame::fromJson(const nlohmann::json &json)
{
    m_separator->setText(QString(json["Separator"].get<char>()));
    m_timestamp_format->setText(json["TimestampFormat"].get<QString>());
    m_ch1_name->setText(json["FirstChannelName"].get<QString>());
    m_ch2_name->setText(json["SecondChannelName"].get<QString>());
}

void LogFormatFrame::toJson(nlohmann::json &json) const
{
    json["Separator"]          = ' ';
    json["TimestampFormat"]    = m_timestamp_format->text();
    json["FirstChannelName"]   = m_ch1_name->text();
    json["SecondChannelName"]  = m_ch2_name->text();
}


void LogFormatFrame::createGui() {
    // ----------[WIDGETS]------------ //
    m_append_en = new QCheckBox(tr("Append file"));
        m_append_en->setChecked(true);

    m_timestamp_en = new QCheckBox(tr("Enable timestamp"));
        m_timestamp_en->setChecked(true);

    m_ch1_en = new QCheckBox(tr("Enable channel 1"));
        m_ch1_en->setChecked(true);

    m_ch2_en = new QCheckBox(tr("Enable channel 2"));
        m_ch2_en->setChecked(true);

    m_timestamp_format = new QLineEdit();
        m_timestamp_format->setPlaceholderText("yyyy.mm.dd hh:mm:ss.sss");

    m_ch1_name = new QLineEdit();
        m_ch1_name->setText("[RX]");
    m_ch2_name = new QLineEdit();
        m_ch2_name->setText("[TX]");

    // ---------[LAYOUT]---------- //
    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        main_layout->addWidget(new QLabel(tr("Log configurations:")), 0, 0, 1, 1);
        main_layout->addWidget(m_append_en, 1, 0, 1, 1);
        main_layout->addWidget(m_timestamp_en, 2, 0, 1, 1);
        main_layout->addWidget(m_ch1_en, 1, 1, 1, 1);
        main_layout->addWidget(m_ch2_en, 2, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Timestamp format")), 3, 0, 1, 1);
        main_layout->addWidget(m_timestamp_format, 3, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Channel 1 label")), 4, 0, 1, 1);
        main_layout->addWidget(m_ch1_name, 4, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Channel 2 label")), 5, 0, 1, 1);
        main_layout->addWidget(m_ch2_name, 5, 1, 1, 1);
        main_layout->setColumnStretch(0, 0);
        main_layout->setColumnStretch(1, 0);
        main_layout->setColumnStretch(2, 0);
        main_layout->setColumnStretch(3, 0);
        main_layout->setColumnStretch(4, 1);

    setLayout(main_layout);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
}

void LogFormatFrame::connectSignals()
{

}


