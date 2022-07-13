#pragma once

#include <QDialog>
#include <QColor>
#include <QFont>

#include <nlohmann/json.hpp>

class QAbstractButton;
class QPushButton;
class QDialogButtonBox;
class QLineEdit;

class LogDecorationDialog: public QDialog
{
    Q_OBJECT

public:
    LogDecorationDialog(QWidget* parent = nullptr);

private:
    void resetGui();
    void applyConfig();
    void createGui();
    void createConnections();

private slots:
    void onDialogButtonClicked(QAbstractButton* btn);
    void onColorButtonClicked();
    void onFontButtonClicked();

private:
    QColor m_attr_color;
    QColor m_cmt_color;
    QColor m_err_color;
    QColor m_ch1_color;
    QColor m_ch2_color;

    QFont m_attr_font;
    QFont m_cmt_font;
    QFont m_err_font;
    QFont m_ch1_font;
    QFont m_ch2_font;

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
