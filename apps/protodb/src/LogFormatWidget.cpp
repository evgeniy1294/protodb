#include "LogFormatWidget.h"
#include "LogFormatter.h"

#include <protodb/utils/JsonUtils.h>

#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

using namespace protodb;

LogFormatWidget::LogFormatWidget(QWidget* aParent)
    : QWidget(aParent)
{
    createGui();
    connectSignals();
}

void LogFormatWidget::defaultConfig(nlohmann::json &json) const
{
    json["CharSeparator"]         = LogFormatter::DefaultSeparator;
    json["Append"]                = true;
    json["TimestampEnabled"]      = true;
    json["ChannelNameEnabled"]    = true;
    json["FirstChannelEnabled"]   = true;
    json["SecondChannelEnabled"]  = true;
    json["CommentChannelEnabled"] = true;
    json["TimestampFormat"]       = LogFormatter::DefaultTimeFormat;
    json["FirstChannelName"]      = QString("RX");
    json["SecondChannelName"]     = QString("TX");
    json["CommentChannelName"]    = QString("USR");
}

void LogFormatWidget::config(nlohmann::json &json) const
{
    json["CharSeparator"]         = ' ';
    json["Append"]                = m_append_en->checkState() == Qt::Checked;
    json["TimestampEnabled"]      = m_timestamp_en->checkState() == Qt::Checked;
    json["ChannelNameEnabled"]    = m_ch_name_en->checkState() == Qt::Checked;
    json["FirstChannelEnabled"]   = m_ch1_en->checkState() == Qt::Checked;
    json["SecondChannelEnabled"]  = m_ch2_en->checkState() == Qt::Checked;
    json["CommentChannelEnabled"] = m_cmt_en->checkState() == Qt::Checked;;
    json["TimestampFormat"]       = m_timestamp_format->text();
    json["FirstChannelName"]      = m_ch1_name->text();
    json["SecondChannelName"]     = m_ch2_name->text();
    json["CommentChannelName"]    = m_cmt_name->text();
}

void LogFormatWidget::setConfig(const nlohmann::json &json)
{
    m_separator->setText(
        QString(json.value<char>("CharSeparator", LogFormatter::DefaultSeparator))
    );

    m_append_en->setCheckState(
        json.value<bool>("Append", true) ? Qt::Checked : Qt::Unchecked
    );

    m_timestamp_en->setCheckState(
        json.value<bool>("TimestampEnabled", true) ? Qt::Checked : Qt::Unchecked
    );

    m_ch_name_en->setCheckState(
        json.value<bool>("ChannelNameEnabled", true) ? Qt::Checked : Qt::Unchecked
    );

    m_ch1_en->setCheckState(
        json.value<bool>("FirstChannelEnabled", true) ? Qt::Checked : Qt::Unchecked
    );

    m_ch2_en->setCheckState(
        json.value<bool>("SecondChannelEnabled", true) ? Qt::Checked : Qt::Unchecked
    );

    m_cmt_en->setCheckState(
        json.value<bool>("CommentChannelEnabled", true) ? Qt::Checked : Qt::Unchecked
    );

    m_timestamp_format->setText(
        json.value("TimestampFormat", LogFormatter::DefaultTimeFormat)
    );

    m_ch1_name->setText(
        json.value("FirstChannelName", QString("RX"))
    );

    m_ch2_name->setText(
        json.value("SecondChannelName", QString("TX"))
    );

    m_cmt_name->setText(
        json.value("CommentChannelName", QString("TX"))
    );


    return;
}

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

    m_cmt_en = new QCheckBox(tr("Comments"));
        m_cmt_en->setChecked(true);

    m_ch_name_en = new QCheckBox(tr("Channel Names"));
        m_ch_name_en->setChecked(true);

    m_timestamp_format = new QLineEdit();
        m_timestamp_format->setPlaceholderText("yyyy.mm.dd hh:mm:ss.zzz");
        m_timestamp_format->setText(LogFormatter::DefaultTimeFormat);

    m_separator = new QLineEdit();
        m_separator->setText(QString(LogFormatter::DefaultSeparator));

    m_ch1_name = new QLineEdit();
        m_ch1_name->setText("RX");
    m_ch2_name = new QLineEdit();
        m_ch2_name->setText("TX");
    m_cmt_name = new QLineEdit();
        m_cmt_name->setText("USR");

    // ---------[LAYOUT]---------- //
    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        main_layout->addWidget(new QLabel(tr("Log configurations:")), 0, 0, 1, 1);
        main_layout->addWidget(m_append_en, 1, 0, 1, 1);
        main_layout->addWidget(m_timestamp_en, 2, 0, 1, 1);
        main_layout->addWidget(m_timestamp_format, 2, 1, 1, 1);
        main_layout->addWidget(m_ch_name_en, 3, 0, 1, 1);
        main_layout->addWidget(m_ch1_en, 4, 0, 1, 1);
        main_layout->addWidget(m_ch1_name, 4, 1, 1, 1);
        main_layout->addWidget(m_ch2_en, 5, 0, 1, 1);
        main_layout->addWidget(m_ch2_name, 5, 1, 1, 1);
        main_layout->addWidget(m_cmt_en, 6, 0, 1, 1);
        main_layout->addWidget(m_cmt_name, 6, 1, 1, 1);
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

    connect(m_ch_name_en, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked) {
            m_ch1_en->setEnabled(true);
            m_ch2_en->setEnabled(true);
            m_cmt_en->setEnabled(true);

            if (m_ch1_en->checkState() == Qt::Checked) {
                m_ch1_name->setEnabled(true);
            }

            if (m_ch2_en->checkState() == Qt::Checked) {
                m_ch2_name->setEnabled(true);
            }

            if (m_cmt_en->checkState() == Qt::Checked) {
                m_cmt_name->setEnabled(true);
            }
        }
        else {
            m_ch1_en->setDisabled(true);
            m_ch2_en->setDisabled(true);
            m_cmt_en->setDisabled(true);
            m_ch1_name->setDisabled(true);
            m_ch2_name->setDisabled(true);
            m_cmt_name->setDisabled(true);
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

    connect(m_cmt_en, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked) {
            m_cmt_name->setEnabled(true);
        }
        else {
            m_cmt_name->setDisabled(true);
        }
    });
}



