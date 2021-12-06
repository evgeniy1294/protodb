#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFontComboBox>
#include <QColorDialog>
#include <QMessageBox>
#include <QCheckBox>
#include "qnlohmann.h"
#include "LogDecorationDialog.h"
#include "LogDecorator.h"

LogDecorationDialog::LogDecorationDialog(QWidget *parent)
    : QDialog(parent)
    , m_decorator(nullptr)
{
    createGui();
    createConnections();
}

void LogDecorationDialog::setDecorator(LogDecorator *decorator)
{
    if (m_decorator != decorator) {

        if (m_decorator != nullptr) {
            disconnect(m_decorator);
        }

        m_decorator = decorator;
        resetGui();
        connect(m_decorator, &LogDecorator::sConfigChanged, this, &LogDecorationDialog::resetGui);
    }
}

void LogDecorationDialog::resetGui()
{
    // All buttons have same size
    QPixmap pixmap(QSize(36,36));

    m_attr_color = m_decorator->attributeColor();
    pixmap.fill(m_attr_color);
    m_attr_color_btn->setIcon(pixmap);

    m_ch1_color = m_decorator->channelColor(kFirstLogChannel);
    pixmap.fill(m_ch1_color);
    m_ch1_color_btn->setIcon(pixmap);

    m_ch2_color = m_decorator->channelColor(kSecondLogChannel);
    pixmap.fill(m_ch2_color);
    m_ch2_color_btn->setIcon(pixmap);

    m_comment_color = m_decorator->channelColor(kCommentLogChannel);
    pixmap.fill(m_comment_color);
    m_comment_color_btn->setIcon(pixmap);

    m_error_color = m_decorator->channelColor(kErrorLogChannel);
    pixmap.fill(m_error_color);
    m_error_color_btn->setIcon(pixmap);

    // Font combo boxes
    QFont font = m_decorator->channelFont(kFirstLogChannel);
    m_ch1_font_cmbx->setCurrentFont(font);

    font = m_decorator->channelFont(kSecondLogChannel);
    m_ch2_font_cmbx->setCurrentFont(font);

    font = m_decorator->channelFont(kCommentLogChannel);
    m_comment_font_cmbx->setCurrentFont(font);

    font = m_decorator->channelFont(kErrorLogChannel);
    m_error_font_cmbx->setCurrentFont(font);

    font = m_decorator->attributeFont();
    m_attr_font_cmbx->setCurrentFont(font);
}

void LogDecorationDialog::applyConfig()
{
    nlohmann::json json;

    json["AttributeColor"]      = m_attr_color;
    json["AttributeFont"]       = m_attr_font_cmbx->currentFont();

    json["FirstChannelColor"]   = m_ch1_color;
    json["FirstChannelFont"]    = m_ch1_font_cmbx->currentFont();

    json["SecondChannelColor"]  = m_ch2_color;
    json["SecondChannelFont"]   = m_ch2_font_cmbx->currentFont();

    json["CommentChannelColor"] = m_comment_color;
    json["CommentChannelFont"]  = m_comment_font_cmbx->currentFont();

    json["ErrorChannelColor"]   = m_error_color;
    json["ErrorChannelFont"]    = m_error_font_cmbx->currentFont();

    m_decorator->fromJson(json);
}

void LogDecorationDialog::createGui()
{
    static const QStringList font_size_list = { "6",  "7",  "8",  "9", "10", "11", "12", "14", "16",
                                               "18", "20", "22", "24", "26", "28", "36", "48", "72"};

    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok |
                                         QDialogButtonBox::Apply |
                                         QDialogButtonBox::Cancel |
                                         QDialogButtonBox::RestoreDefaults |
                                         QDialogButtonBox::Reset );

    // Color buttons
    m_attr_color_btn = new QPushButton();
    m_comment_color_btn = new QPushButton();
    m_error_color_btn = new QPushButton();
    m_ch1_color_btn = new QPushButton();
    m_ch2_color_btn = new QPushButton();

    // Font ComboBoxes
    m_attr_font_cmbx    = new QFontComboBox();
    m_comment_font_cmbx = new QFontComboBox();
    m_error_font_cmbx   = new QFontComboBox();
    m_ch1_font_cmbx = new QFontComboBox();
    m_ch2_font_cmbx = new QFontComboBox();

    // Font size ComboBoxes
    m_attr_font_size_cmbx = new QComboBox();
        m_attr_font_size_cmbx->addItems(font_size_list);

    m_comment_font_size_cmbx = new QComboBox();
        m_comment_font_size_cmbx->addItems(font_size_list);

    m_error_font_size_cmbx   = new QComboBox();
        m_error_font_size_cmbx->addItems(font_size_list);

    m_ch1_font_size_cmbx = new QComboBox();
        m_ch1_font_size_cmbx->addItems(font_size_list);

    m_ch2_font_size_cmbx = new QComboBox();
        m_ch2_font_size_cmbx->addItems(font_size_list);

    // Font style CheckBoxes
    auto m_attr_font_bold_cb = new QCheckBox("B");
        auto font = m_attr_font_bold_cb->font();
        font.setBold(true);
    m_attr_font_bold_cb->setFont(font);

    auto m_attr_font_italic_cb = new QCheckBox("I");
        font = m_attr_font_italic_cb->font();
        font.setItalic(true);
    m_attr_font_italic_cb->setFont(font);

    auto m_attr_font_underline_cb = new QCheckBox("U");
        font = m_attr_font_underline_cb->font();
        font.setUnderline(true);
    m_attr_font_underline_cb->setFont(font);

    // Layout
    auto inner_layout = new QGridLayout();
        inner_layout->addWidget( new QLabel(tr("First channel")), 0, 0 );
        inner_layout->addWidget(m_ch1_color_btn, 0, 1);
        inner_layout->addWidget(m_ch1_font_cmbx, 0, 2);
        inner_layout->addWidget(m_ch1_font_size_cmbx, 0, 3);
        inner_layout->addWidget(m_attr_font_bold_cb, 0, 4);
        inner_layout->addWidget(m_attr_font_italic_cb, 0, 5);
        inner_layout->addWidget(m_attr_font_underline_cb, 0, 6);

        inner_layout->addWidget( new QLabel(tr("Second channel")), 1, 0 );
        inner_layout->addWidget(m_ch2_color_btn, 1, 1);
        inner_layout->addWidget(m_ch2_font_cmbx, 1, 2);
        inner_layout->addWidget(m_ch2_font_size_cmbx, 1, 3);

        inner_layout->addWidget( new QLabel(tr("Attribute")), 2, 0 );
        inner_layout->addWidget(m_attr_color_btn, 2, 1);
        inner_layout->addWidget(m_attr_font_cmbx, 2, 2);
        inner_layout->addWidget(m_attr_font_size_cmbx, 2, 3);

        inner_layout->addWidget( new QLabel(tr("Comments")), 3, 0 );
        inner_layout->addWidget(m_comment_color_btn, 3, 1);
        inner_layout->addWidget(m_comment_font_cmbx, 3, 2);
        inner_layout->addWidget(m_comment_font_size_cmbx, 3, 3);

        inner_layout->addWidget( new QLabel(tr("Errors")), 4, 0 );
        inner_layout->addWidget(m_error_color_btn, 4, 1);
        inner_layout->addWidget(m_error_font_cmbx, 4, 2);
        inner_layout->addWidget(m_error_font_size_cmbx, 4, 3);


        inner_layout->setAlignment(Qt::AlignTop);
        inner_layout->setColumnStretch(0, 0);
        inner_layout->setColumnStretch(1, 0);
        inner_layout->setColumnStretch(2, 1);
        inner_layout->setColumnStretch(3, 0);

    auto main_layout = new QVBoxLayout();
        main_layout->addLayout(inner_layout);
        main_layout->addWidget(m_dialog_btn);
    setLayout(main_layout);
}

void LogDecorationDialog::createConnections()
{
    connect(m_dialog_btn, &QDialogButtonBox::clicked, this, &LogDecorationDialog::onDialogButtonClicked);
    connect(m_attr_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_comment_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_error_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_ch1_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_ch2_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
}

void LogDecorationDialog::onDialogButtonClicked(QAbstractButton* btn)
{
    switch( m_dialog_btn->standardButton( btn ) )
    {
        case QDialogButtonBox::Apply:
            applyConfig();
            break;

        case QDialogButtonBox::Ok:
            applyConfig();
            accept();
            break;

        case QDialogButtonBox::Reset:
            resetGui();
            break;

        case QDialogButtonBox::RestoreDefaults: {
            QMessageBox msgbox;
            {
                msgbox.setText( tr("Reset configurations to default") );
                msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgbox.setDefaultButton(QMessageBox::No);
                msgbox.setIcon(QMessageBox::Icon::Warning);
                msgbox.setFixedSize( QSize(680, 240) );

                    msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
                    msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
            }

            if (msgbox.exec() == QMessageBox::Yes) {
                m_decorator->setDefaultConfig();
            }
        } break;

        case QDialogButtonBox::Cancel:
            hide();
            break;

        default:
            break;
    }
}

void LogDecorationDialog::onColorButtonClicked()
{
    QColorDialog dialog;
    {
       dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    }

    if (dialog.exec() == 1) {
        auto color = dialog.selectedColor();
        auto pixmap = QPixmap(QSize(36,36));
            pixmap.fill(color);

        auto signalSender = sender();

        if (signalSender == m_attr_color_btn) {
            m_attr_color = color;
            m_attr_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_comment_color_btn) {
            m_comment_color = color;
            m_comment_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_error_color_btn) {
            m_error_color = color;
            m_error_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_ch1_color_btn) {
            m_ch1_color = color;
            m_ch1_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_ch2_color_btn) {
            m_ch2_color = color;
            m_ch2_color_btn->setIcon(QIcon(pixmap));
        }
    }
}
