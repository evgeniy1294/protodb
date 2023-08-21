#pragma once

#include <QWidget>

class QAction;
class QComboBox;
class QLabel;
class QPlainTextEdit;
class QToolButton;

namespace Okteta {
    class PieceTableByteArrayModel;
    class ByteArrayColumnView;
}

namespace protodb {

class BytecodeEditor: public QWidget
{
    Q_OBJECT

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

    BytecodeEditor(QWidget* parent = nullptr);
   ~BytecodeEditor() = default;

    void setDisplayFormat(DisplayFormat format);
    DisplayFormat displayFormat() const;

    void setDisplayMode(DisplayMode mode);
    DisplayMode displayMode() const;

    void setData(const QByteArray& data);
    QByteArray currentData() const;

private:
    void createActions();
    void createGui();
    void connectSignals();

private:
    void paintEvent(QPaintEvent *event) override;

private:
    DisplayFormat m_format;
    DisplayMode m_mode;

    Okteta::PieceTableByteArrayModel* m_model;
    Okteta::ByteArrayColumnView* m_view;

    QComboBox* m_codecs;

    QLabel* m_counter_label;

    QToolButton* m_format_btn;
    QToolButton* m_mode_btn;

    QAction* m_set_hexmode_act;
    QAction* m_set_decmode_act;
    QAction* m_set_octmode_act;
    QAction* m_set_binmode_act;
    QAction* m_set_code_text_act;
    QAction* m_set_code_only_act;
    QAction* m_set_text_only_act;
};


} // namespace protodb
