#pragma once

#include "Logger.h"

#include <QDialog>
#include <QColor>
#include <QFont>

class QAbstractButton;
class QPushButton;
class QDialogButtonBox;
class QLineEdit;

namespace protodb {

class LogDecorator;

class LogDecorationDialog: public QDialog
{
    Q_OBJECT

public:
    LogDecorationDialog(QWidget* parent = nullptr);

    void apply(LogDecorator* dec);
    void readOrigin(LogDecorator* dec);

signals:
    void sConfigChanged();

private:
    void applyValue();
    void restoreDefaultValue();
    void restoreValue();
    void resetGui();
    void createGui();
    void createConnections();

private slots:
    void onDialogButtonClicked(QAbstractButton* btn);
    void onColorButtonClicked();
    void onFontButtonClicked();

private:
    QColor m_attr_color;
    QColor m_temp_attr_color;

    QMap<Logger::Channel, QColor> m_ch_colors;
    QMap<Logger::Channel, QColor> m_temp_ch_colors;

    QMap<Logger::Channel, QFont> m_ch_fonts;
    QMap<Logger::Channel, QFont> m_temp_ch_fonts;

private:
    QDialogButtonBox* m_dialog_btn;

    QPushButton* m_ch1_color_btn;
    QPushButton* m_ch2_color_btn;
    QPushButton* m_cmt_color_btn;
    QPushButton* m_attr_color_btn;
    QPushButton* m_err_color_btn;

    QPushButton* m_ch1_font_btn;
    QPushButton* m_ch2_font_btn;
    QPushButton* m_cmt_font_btn;
    QPushButton* m_attr_font_btn;
    QPushButton* m_err_font_btn;

    QLineEdit* m_ch1_font_le;
    QLineEdit* m_ch2_font_le;
    QLineEdit* m_err_font_le;
    QLineEdit* m_cmt_font_le;
    QLineEdit* m_attr_font_le;
};

} // namespace protodb
