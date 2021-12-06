#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFontComboBox>
#include <QColorDialog>
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
    m_decorator = decorator;
    resetGui();
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
    m_attr_font_cmbx->setFont(m_decorator->attributeFont());
    m_comment_font_cmbx->setFont(m_decorator->channelFont(kCommentLogChannel));
    m_error_font_cmbx->setFont(m_decorator->channelFont(kErrorLogChannel));
    m_ch1_font_cmbx->setFont(m_decorator->channelFont(kFirstLogChannel));
    m_ch2_font_cmbx->setFont(m_decorator->channelFont(kSecondLogChannel));
}

void LogDecorationDialog::applyConfig()
{
    nlohmann::json json;

    json["AttributeColor"]      = m_attr_color;
    json["AttributeFont"]       = m_attr_font_cmbx->font();

    json["FirstChannelColor"]   = m_ch1_color;
    json["FirstChannelFont"]    = m_ch1_font_cmbx->font();

    json["SecondChannelColor"]  = m_ch2_color;
    json["SecondChannelFont"]   = m_ch2_font_cmbx->font();

    json["CommentChannelColor"] = m_comment_color;
    json["CommentChannelFont"]  = m_comment_font_cmbx->font();

    json["ErrorChannelColor"]   = m_error_color;
    json["ErrorChannelFont"]    = m_error_font_cmbx->font();

    m_decorator->fromJson(json);
}

void LogDecorationDialog::createGui()
{
    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok |
                                         QDialogButtonBox::Apply |
                                         QDialogButtonBox::Cancel |
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

    // Layout
    auto main_layout = new QGridLayout();
        main_layout->addWidget( new QLabel(tr("First channel")), 0, 0 );
        main_layout->addWidget(m_ch1_color_btn, 0, 1);
        main_layout->addWidget(m_ch1_font_cmbx, 0, 2);

        main_layout->addWidget( new QLabel(tr("Second channel")), 1, 0 );
        main_layout->addWidget(m_ch2_color_btn, 1, 1);
        main_layout->addWidget(m_ch2_font_cmbx, 1, 2);

        main_layout->addWidget( new QLabel(tr("Attribute")), 2, 0 );
        main_layout->addWidget(m_attr_color_btn, 2, 1);
        main_layout->addWidget(m_attr_font_cmbx, 2, 2);

        main_layout->addWidget( new QLabel(tr("Comments")), 3, 0 );
        main_layout->addWidget(m_comment_color_btn, 3, 1);
        main_layout->addWidget(m_comment_font_cmbx, 3, 2);

        main_layout->addWidget( new QLabel(tr("Errors")), 4, 0 );
        main_layout->addWidget(m_error_color_btn, 4, 1);
        main_layout->addWidget(m_error_font_cmbx, 4, 2);

        main_layout->addWidget(m_dialog_btn, 5, 0, 1, 3);

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
            resetGui(); // Может стоит использовать настройки по-умолчанию? Есть кнопка RestoreDefaults
            break;

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
