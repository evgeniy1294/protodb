#pragma once

#include <QDialog>
#include <QColor>
#include <QFont>

class QAbstractButton;
class QPushButton;
class QDialogButtonBox;
class QFontComboBox;
class QComboBox;

class LogDecorator;

class LogDecorationDialog: public QDialog // , public Configurable
{
    Q_OBJECT

public:
    LogDecorationDialog(QWidget* parent = nullptr);

    /* void defaultConfig(nlohmann::json& json) const override;
       void toJson(nlohmann::json& json) const override;
       void fromJson(const nlohmann::json& json) override; */
    void setDecorator(LogDecorator* decorator);



private:
    void resetGui();
    void applyConfig();
    void createGui();
    void createConnections();

private slots:
    void onDialogButtonClicked(QAbstractButton* btn);
    void onColorButtonClicked();

private:
    LogDecorator* m_decorator;

    QColor m_attr_color;
    QColor m_comment_color;
    QColor m_error_color;
    QColor m_ch1_color;
    QColor m_ch2_color;

private:

    QDialogButtonBox* m_dialog_btn;

    QPushButton* m_attr_color_btn;
    QPushButton* m_comment_color_btn;
    QPushButton* m_error_color_btn;
    QPushButton* m_ch1_color_btn;
    QPushButton* m_ch2_color_btn;

    QFontComboBox* m_attr_font_cmbx;
    QFontComboBox* m_comment_font_cmbx;
    QFontComboBox* m_error_font_cmbx;
    QFontComboBox* m_ch1_font_cmbx;
    QFontComboBox* m_ch2_font_cmbx;

    QComboBox* m_attr_font_size_cmbx;
    QComboBox* m_comment_font_size_cmbx;
    QComboBox* m_error_font_size_cmbx;
    QComboBox* m_ch1_font_size_cmbx;
    QComboBox* m_ch2_font_size_cmbx;
};
