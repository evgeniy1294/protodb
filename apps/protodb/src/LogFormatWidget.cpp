#include "LogFormatWidget.h"

#include <protodb/utils/JsonUtils.h>

#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

LogFormatWidget::LogFormatWidget(QWidget* aParent)
    : QWidget(aParent)
{
    createGui();
    connectSignals();
}

/*
void LogFormatWidget::defaultConfig(nlohmann::json &json) const
{
    json["CharSeparator"]        = ' ';
    json["TimestampEnabled"]     = true;
    json["FirstChannelEnabled"]  = true;
    json["SecondChannelEnabled"] = true;
    json["TimestampFormat"]      = QString("hh:mm:ss.sss");
    json["FirstChannelName"]     = QString("RX");
    json["SecondChannelName"]    = QString("TX");
}

void LogFormatWidget::fromJson(const nlohmann::json &json)
{
    m_separator->setText(QString(json["CharSeparator"].get<char>()));
    m_timestamp_en->setCheckState(json["TimestampEnabled"].get<bool>() ? Qt::Checked : Qt::Unchecked);
    m_timestamp_en->setCheckState(json["FirstChannelEnabled"].get<bool>() ? Qt::Checked : Qt::Unchecked);
    m_timestamp_en->setCheckState(json["SecondChannelEnabled"].get<bool>() ? Qt::Checked : Qt::Unchecked);
    m_timestamp_format->setText(json["TimestampFormat"].get<QString>());
    m_ch1_name->setText(json["FirstChannelName"].get<QString>());
    m_ch2_name->setText(json["SecondChannelName"].get<QString>());
}

void LogFormatWidget::toJson(nlohmann::json &json) const
{
    json["CharSeparator"]        = ' '; /// TODO: Задействовать
    json["TimestampEnabled"]     = m_timestamp_en->checkState() == Qt::Checked;
    json["FirstChannelEnabled"]  = m_ch1_en->checkState() == Qt::Checked;
    json["SecondChannelEnabled"] = m_ch2_en->checkState() == Qt::Checked;
    json["TimestampFormat"]      = m_timestamp_format->text();
    json["FirstChannelName"]     = m_ch1_name->text();
    json["SecondChannelName"]    = m_ch2_name->text();
}
*/

void LogFormatWidget::createGui() {
    // ----------[WIDGETS]------------ //
    m_append_en = new QCheckBox(tr("Append file"));
        m_append_en->setChecked(true);

    m_timestamp_en = new QCheckBox(tr("Timestamp"));
        m_timestamp_en->setChecked(true);

    m_ch1_en = new QCheckBox(tr("Channel 1"));
        m_ch1_en->setChecked(true);

    m_ch2_en = new QCheckBox(tr("Channel 2"));
        m_ch2_en->setChecked(true);

    m_timestamp_format = new QLineEdit();
        m_timestamp_format->setPlaceholderText("yyyy.mm.dd hh:mm:ss.sss");
        m_timestamp_format->setText("hh:mm:ss.sss");

    m_ch1_name = new QLineEdit();
        m_ch1_name->setText("RX");
    m_ch2_name = new QLineEdit();
        m_ch2_name->setText("TX");

    // ---------[LAYOUT]---------- //
    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        main_layout->addWidget(new QLabel(tr("Log configurations:")), 0, 0, 1, 1);
        main_layout->addWidget(m_append_en, 1, 0, 1, 1);
        main_layout->addWidget(m_timestamp_en, 2, 0, 1, 1);
        main_layout->addWidget(m_timestamp_format, 2, 1, 1, 1);
        main_layout->addWidget(m_ch1_en, 3, 0, 1, 1);
        main_layout->addWidget(m_ch1_name, 3, 1, 1, 1);
        main_layout->addWidget(m_ch2_en, 4, 0, 1, 1);
        main_layout->addWidget(m_ch2_name, 4, 1, 1, 1);
        main_layout->setColumnStretch(0, 0);
        main_layout->setColumnStretch(1, 1);

    setLayout(main_layout);
}

void LogFormatWidget::connectSignals()
{
    connect(m_timestamp_en, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked) {
            m_timestamp_format->setEnabled(true);
        }
        else {
            m_timestamp_format->setDisabled(true);
        }
    });

    connect(m_ch1_en, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked) {
            m_ch1_name->setEnabled(true);
        }
        else {
            m_ch1_name->setDisabled(true);
        }
    });

    connect(m_ch2_en, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked) {
            m_ch2_name->setEnabled(true);
        }
        else {
            m_ch2_name->setDisabled(true);
        }
    });
}



