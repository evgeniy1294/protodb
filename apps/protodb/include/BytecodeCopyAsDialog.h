#pragma once

#include <QDialog>
#include <QMap>

class QAbstractButton;
class QComboBox;
class QDialogButtonBox;
class QGridLayout;
class QPlainTextEdit;

namespace protodb {

class BytecodeEncoderAbstract;
class ConfigWidgetAbstract;

class BytecodeCopyAsDialog final: public QDialog
{
    Q_OBJECT

public:
    BytecodeCopyAsDialog(QWidget* parent = nullptr);
    ~BytecodeCopyAsDialog() = default;

    void setEncoder(BytecodeEncoderAbstract* encoder, ConfigWidgetAbstract* widget);
    void setData(const QByteArray& data);

private slots:
    void onDialogClicked(QAbstractButton* btn);
    void updatePreview();

private:
    void createGui();
    void connectSignals();

private:
    QByteArray m_data;

    BytecodeEncoderAbstract* m_encoder;
    ConfigWidgetAbstract* m_cofig_widget;

    QWidget* m_dummy_wgt;
    QDialogButtonBox* m_dialog_btn;
    QPlainTextEdit* m_preview;
    QComboBox* m_encoder_cbx;
    QGridLayout* m_layout;
};


} // namespace protodb
