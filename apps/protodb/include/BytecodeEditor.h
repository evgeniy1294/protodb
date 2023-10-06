#pragma once

#include <QWidget>
#include <QMap>

class QAction;
class QComboBox;
class QLabel;
class QPlainTextEdit;
class QToolButton;
class QMenu;

namespace Okteta {
    class PieceTableByteArrayModel;
    class ByteArrayColumnView;
}

namespace protodb {

class BytecodeCharsEncoder;
class BytecodeValuesEncoder;
class BytecodeSourceCodeEncoder;
class BytecodeCopyAsDialog;

class BytecodeEditor: public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QByteArray currentData READ currentData WRITE setData CONSTANT)

public:
    enum DisplayFormat {
        HexFormat,
        DecimalFormat,
        OctalFormat,
        BinaryFormat,
        AsciiFormat,

        FormatCount
    };

    enum DisplayMode {
        CodePlusTextMode,
        CodeOnlyMode,
        TextOnlyMode,

        ModeCount
    };

    enum ColumnWidth {
        One = 1, Two = 2, Four = 4, Eight = 8
    };

    BytecodeEditor(QWidget* parent = nullptr);
   ~BytecodeEditor() = default;

    void setDisplayFormat(DisplayFormat format);
    DisplayFormat displayFormat() const;

    void setDisplayMode(DisplayMode mode);
    DisplayMode displayMode() const;

    void setColumnWidth(ColumnWidth width);
    ColumnWidth columnWidth() const;

    void setData(const QByteArray& data);
    QByteArray currentData() const;

private:
    void createActions();
    void createEncoders();
    void createGui();
    void connectSignals();

private:
    DisplayFormat m_format;
    DisplayMode m_mode;
    ColumnWidth m_width;

    Okteta::PieceTableByteArrayModel* m_model;
    Okteta::ByteArrayColumnView* m_view;


    QComboBox* m_codecs;

    QLabel* m_counter_label;

    QToolButton* m_format_btn;
    QToolButton* m_mode_btn;
    QToolButton* m_width_btn;

    QAction* m_set_hexmode_act;
    QAction* m_set_decmode_act;
    QAction* m_set_octmode_act;
    QAction* m_set_binmode_act;
    QAction* m_set_code_text_act;
    QAction* m_set_code_only_act;
    QAction* m_set_text_only_act;
    QAction* m_set_width_1_act;
    QAction* m_set_width_2_act;
    QAction* m_set_width_4_act;
    QAction* m_set_width_8_act;
    QAction* m_encode_as_values_act;
    QAction* m_encode_as_chars_act;
    QAction* m_encode_as_code_act;

    QMenu* m_copy_as_menu;

    BytecodeValuesEncoder* m_values_encoder;
    BytecodeCharsEncoder* m_chars_encoder;
    BytecodeSourceCodeEncoder* m_code_encoder;
    BytecodeCopyAsDialog* m_copy_as_dialog;
};


} // namespace protodb
