#include "LogDecorationDialog.h"

#include "Logger.h"
#include "LogDecorator.h"
#include <protodb/utils/JsonUtils.h>

#include <QApplication>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>

LogDecorationDialog::LogDecorationDialog(QWidget *parent)
    : QDialog(parent)
{
    createGui();
    createConnections();

    restoreDefaultValue();
}

void LogDecorationDialog::apply(LogDecorator* dec)
{
    dec->setAttributeColor(m_attr_color);

}

void LogDecorationDialog::applyValue()
{
    m_attr_color = m_temp_attr_color;
    m_ch_colors  = m_temp_ch_colors;

    m_attr_font  = m_temp_attr_font;
    m_ch_fonts   = m_temp_ch_fonts;

    emit sConfigChanged();
}

void LogDecorationDialog::restoreDefaultValue()
{
    m_attr_color = LogDecorator::defaultAttributeColor();
    m_ch_colors = LogDecorator::defaultChannelColors();

    m_attr_font  = LogDecorator::defaultAttributeFont();
    m_ch_fonts  = LogDecorator::defaultChannelFonts();

    restoreValue();
    emit sConfigChanged();
}

void LogDecorationDialog::restoreValue()
{
    m_temp_attr_color = m_attr_color;
    m_temp_ch_colors  = m_ch_colors;
    m_temp_attr_font  = m_attr_font;
    m_temp_ch_fonts   = m_ch_fonts;

    resetGui();
}


void LogDecorationDialog::resetGui()
{
    // All buttons have same size
    QPixmap pixmap(QSize(36,36));

    pixmap.fill(m_attr_color);
    m_attr_color_btn->setIcon(pixmap);

    pixmap.fill(m_ch_colors.value(Logger::ChannelFirst, LogDecorator::defaultChannelColor(Logger::ChannelFirst)));
    m_ch1_color_btn->setIcon(pixmap);

    pixmap.fill(m_ch_colors.value(Logger::ChannelSecond, LogDecorator::defaultChannelColor(Logger::ChannelSecond)));
    m_ch2_color_btn->setIcon(pixmap);

    pixmap.fill(m_ch_colors.value(Logger::ChannelComment, LogDecorator::defaultChannelColor(Logger::ChannelComment)));
    m_cmt_color_btn->setIcon(pixmap);

    pixmap.fill(m_ch_colors.value(Logger::ChannelError, LogDecorator::defaultChannelColor(Logger::ChannelError)));
    m_err_color_btn->setIcon(pixmap);

    // Font combo boxes
    auto font = m_ch_fonts.value(Logger::ChannelFirst, LogDecorator::defaultChannelFont(Logger::ChannelFirst));
    QString font_str = QString("%1, %2").arg(font.family()).arg(font.pointSize());
        m_ch1_font_le->setFont(font);
        m_ch1_font_le->setText(font_str);

    font = m_ch_fonts.value(Logger::ChannelSecond, LogDecorator::defaultChannelFont(Logger::ChannelSecond));
    font_str = QString("%1, %2").arg(font.family()).arg(font.pointSize());
        m_ch2_font_le->setFont(font);
        m_ch2_font_le->setText(font_str);

    font = m_ch_fonts.value(Logger::ChannelComment, LogDecorator::defaultChannelFont(Logger::ChannelComment));
    font_str = QString("%1, %2").arg(font.family()).arg(font.pointSize());
        m_cmt_font_le->setFont(font);
        m_cmt_font_le->setText(font_str);

    font = m_ch_fonts.value(Logger::ChannelError, LogDecorator::defaultChannelFont(Logger::ChannelError));
    font_str = QString("%1, %2").arg(font.family()).arg(font.pointSize());
        m_err_font_le->setFont(font);
        m_err_font_le->setText(font_str);

    font_str = QString("%1, %2").arg(m_attr_font.family()).arg(m_attr_font.pointSize());
        m_attr_font_le->setFont(m_attr_font);
        m_attr_font_le->setText(font_str);
}


void LogDecorationDialog::createGui()
{
    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok |
                                         QDialogButtonBox::Apply |
                                         QDialogButtonBox::Cancel |
                                         QDialogButtonBox::RestoreDefaults |
                                         QDialogButtonBox::Reset );

    // Color buttons
    m_attr_color_btn = new QPushButton();
    m_cmt_color_btn  = new QPushButton();
    m_err_color_btn  = new QPushButton();
    m_ch1_color_btn  = new QPushButton();
    m_ch2_color_btn  = new QPushButton();

    // Font lines
    QString font_str_default = QString("%1, %2").arg(QApplication::font().family()).arg(QApplication::font().pointSize());

    m_ch1_font_le  = new QLineEdit();
        m_ch1_font_le->setReadOnly(true);
        m_ch1_font_le->setText(font_str_default);
    m_ch2_font_le  = new QLineEdit();
        m_ch2_font_le->setReadOnly(true);
        m_ch2_font_le->setText(font_str_default);
    m_err_font_le  = new QLineEdit();
        m_err_font_le->setReadOnly(true);
        m_err_font_le->setText(font_str_default);
    m_cmt_font_le  = new QLineEdit();
        m_cmt_font_le->setReadOnly(true);
        m_cmt_font_le->setText(font_str_default);
    m_attr_font_le = new QLineEdit();
        m_attr_font_le->setReadOnly(true);
        m_attr_font_le->setText(font_str_default);

    // Font buttons
    m_attr_font_btn = new QPushButton();
        m_attr_font_btn->setIcon(QIcon(":/icons/edit.svg"));

    m_cmt_font_btn = new QPushButton();
        m_cmt_font_btn->setIcon(QIcon(":/icons/edit.svg"));

    m_err_font_btn = new QPushButton();
        m_err_font_btn->setIcon(QIcon(":/icons/edit.svg"));

    m_ch1_font_btn = new QPushButton();
        m_ch1_font_btn->setIcon(QIcon(":/icons/edit.svg"));

    m_ch2_font_btn = new QPushButton();
        m_ch2_font_btn->setIcon(QIcon(":/icons/edit.svg"));

    // Layout
    auto inner_layout = new QGridLayout();
        inner_layout->addWidget( new QLabel(tr("First channel")), 0, 0 );
        inner_layout->addWidget(m_ch1_font_le, 0, 1);
        inner_layout->addWidget(m_ch1_font_btn, 0, 2);
        inner_layout->addWidget(m_ch1_color_btn, 0, 3);


        inner_layout->addWidget( new QLabel(tr("Second channel")), 1, 0 );
        inner_layout->addWidget(m_ch2_font_le, 1, 1);
        inner_layout->addWidget(m_ch2_font_btn, 1, 2);
        inner_layout->addWidget(m_ch2_color_btn, 1, 3);

        inner_layout->addWidget( new QLabel(tr("Attribute")), 2, 0 );
        inner_layout->addWidget(m_attr_font_le, 2, 1);
        inner_layout->addWidget(m_attr_font_btn, 2, 2);
        inner_layout->addWidget(m_attr_color_btn, 2, 3);

        inner_layout->addWidget( new QLabel(tr("Comments")), 3, 0 );
        inner_layout->addWidget(m_cmt_font_le, 3, 1);
        inner_layout->addWidget(m_cmt_font_btn, 3, 2);
        inner_layout->addWidget(m_cmt_color_btn, 3, 3);

        inner_layout->addWidget( new QLabel(tr("Errors")), 4, 0 );
        inner_layout->addWidget(m_err_font_le, 4, 1);
        inner_layout->addWidget(m_err_font_btn, 4, 2);
        inner_layout->addWidget(m_err_color_btn, 4, 3);


        inner_layout->setAlignment(Qt::AlignTop);
        inner_layout->setColumnStretch(0, 0);
        inner_layout->setColumnStretch(1, 1);
        inner_layout->setColumnStretch(2, 0);
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
    connect(m_cmt_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_err_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_ch1_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_ch2_color_btn, &QPushButton::clicked, this, &LogDecorationDialog::onColorButtonClicked);
    connect(m_attr_font_btn, &QPushButton::clicked, this, &LogDecorationDialog::onFontButtonClicked);
    connect(m_cmt_font_btn, &QPushButton::clicked, this, &LogDecorationDialog::onFontButtonClicked);
    connect(m_err_font_btn, &QPushButton::clicked, this, &LogDecorationDialog::onFontButtonClicked);
    connect(m_ch1_font_btn, &QPushButton::clicked, this, &LogDecorationDialog::onFontButtonClicked);
    connect(m_ch2_font_btn, &QPushButton::clicked, this, &LogDecorationDialog::onFontButtonClicked);
}

void LogDecorationDialog::onDialogButtonClicked(QAbstractButton* btn)
{
    switch( m_dialog_btn->standardButton( btn ) )
    {
        case QDialogButtonBox::Apply:
            applyValue();
            break;

        case QDialogButtonBox::Ok:
            applyValue();
            accept();
            break;

        case QDialogButtonBox::Reset:
            restoreValue();
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
                restoreDefaultValue();
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
    auto signalSender = sender();
    QColor previos_color;

    if (signalSender == m_attr_color_btn) {
        previos_color = m_temp_attr_color;
    }
    else if (signalSender == m_cmt_color_btn) {
        previos_color = m_temp_ch_colors.value(Logger::ChannelComment, LogDecorator::defaultChannelColor(Logger::ChannelComment));
    }
    else if (signalSender == m_err_color_btn) {
        previos_color = m_temp_ch_colors.value(Logger::ChannelError, LogDecorator::defaultChannelColor(Logger::ChannelError));
    }
    else if (signalSender == m_ch1_color_btn) {
        previos_color = m_temp_ch_colors.value(Logger::ChannelFirst, LogDecorator::defaultChannelColor(Logger::ChannelFirst));
    }
    else if (signalSender == m_ch2_color_btn) {
        previos_color = m_temp_ch_colors.value(Logger::ChannelSecond, LogDecorator::defaultChannelColor(Logger::ChannelSecond));
    }

    QColorDialog dialog;
    {
        dialog.setCurrentColor(previos_color);
        dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    }

    if (dialog.exec() == 1) {
        auto color = dialog.selectedColor();
        auto pixmap = QPixmap(QSize(36,36));
            pixmap.fill(color);

        auto signalSender = sender();

        if (signalSender == m_attr_color_btn) {
            m_temp_attr_color = color;
            m_attr_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_cmt_color_btn) {
            m_temp_ch_colors[Logger::ChannelComment] = color;
            m_cmt_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_err_color_btn) {
            m_temp_ch_colors[Logger::ChannelError] = color;
            m_err_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_ch1_color_btn) {
            m_temp_ch_colors[Logger::ChannelFirst] = color;
            m_ch1_color_btn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == m_ch2_color_btn) {
            m_temp_ch_colors[Logger::ChannelSecond] = color;
            m_ch2_color_btn->setIcon(QIcon(pixmap));
        }
    }
}

void LogDecorationDialog::onFontButtonClicked()
{
    auto signalSender = sender();
    QFont previos_font;

    if (signalSender == m_attr_font_btn) {
        previos_font = m_temp_attr_font;
    }
    else if (signalSender == m_cmt_font_btn) {
        previos_font = m_temp_ch_fonts.value(Logger::ChannelComment, LogDecorator::defaultChannelFont(Logger::ChannelComment));
    }
    else if (signalSender == m_err_font_btn) {
        previos_font = m_temp_ch_fonts.value(Logger::ChannelError, LogDecorator::defaultChannelFont(Logger::ChannelError));
    }
    else if (signalSender == m_ch1_font_btn) {
        previos_font = m_temp_ch_fonts.value(Logger::ChannelFirst, LogDecorator::defaultChannelFont(Logger::ChannelFirst));
    }
    else if (signalSender == m_ch2_font_btn) {
        previos_font = m_temp_ch_fonts.value(Logger::ChannelSecond, LogDecorator::defaultChannelFont(Logger::ChannelSecond));
    }

    QFontDialog dialog;
    {
        dialog.setMinimumSize(QSize(640, 480));
        dialog.setCurrentFont(previos_font);
    }

    if (dialog.exec() == 1) {
        QFont font = dialog.selectedFont();
        QString font_str = QString("%1, %2").arg(font.family()).arg(font.pointSize());

        if (signalSender == m_attr_font_btn) {
            m_temp_attr_font = font;
            m_attr_font_le->setFont(font);
            m_attr_font_le->setText(font_str);

        }
        else if (signalSender == m_cmt_font_btn) {
            m_temp_ch_fonts[Logger::ChannelComment] = font;
            m_cmt_font_le->setFont(font);
            m_cmt_font_le->setText(font_str);
        }
        else if (signalSender == m_err_font_btn) {
            m_temp_ch_fonts[Logger::ChannelError] = font;
            m_err_font_le->setFont(font);
            m_err_font_le->setText(font_str);
        }
        else if (signalSender == m_ch1_font_btn) {
            m_temp_ch_fonts[Logger::ChannelFirst] = font;
            m_ch1_font_le->setFont(font);
            m_ch1_font_le->setText(font_str);
        }
        else if (signalSender == m_ch2_font_btn) {
            m_temp_ch_fonts[Logger::ChannelSecond] = font;
            m_ch2_font_le->setFont(font);
            m_ch2_font_le->setText(font_str);
        }
    }
}
