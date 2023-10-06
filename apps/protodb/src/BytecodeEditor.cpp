#include "BytecodeEditor.h"

#include "BytecodeCharsEncoder.h"
#include "BytecodeValuesEncoder.h"
#include "BytecodeSourceCodeEncoder.h"
#include "BytecodeCopyAsDialog.h"

#include <Okteta/CharCodec.hpp>
#include <Okteta/ByteArrayColumnView.hpp>
#include <Okteta/PieceTableByteArrayModel.hpp>

#include <QApplication>
#include <QLabel>
#include <QAction>
#include <QComboBox>
#include <QMenu>
#include <QProxyStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>

namespace protodb {

class MenuProxyStyle final: public QProxyStyle
{
public:
    explicit MenuProxyStyle(QStyle *style = nullptr): QProxyStyle(style) { }

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override {
        if (metric == PixelMetric::PM_SmallIconSize) {
            return 24;
        }

        return QProxyStyle::pixelMetric(metric, option, widget);
    }
};

} // protodb

using namespace protodb;


BytecodeEditor::BytecodeEditor(QWidget* parent)
    : QWidget(parent)
    , m_format(HexFormat)
    , m_mode(CodePlusTextMode)
    , m_width(ColumnWidth::Four)
{
    m_model = new Okteta::PieceTableByteArrayModel(QByteArray(), this);

    createActions();
    createGui();
    connectSignals();
    createEncoders();
}

void BytecodeEditor::setDisplayFormat(DisplayFormat format)
{
    switch(format) {
        case HexFormat:
            m_format_btn->setDefaultAction(m_set_hexmode_act);
            m_view->setValueCoding(Okteta::AbstractByteArrayView::HexadecimalCoding);
            break;
        case DecimalFormat:
            m_format_btn->setDefaultAction(m_set_decmode_act);
            m_view->setValueCoding(Okteta::AbstractByteArrayView::DecimalCoding);
            break;
        case OctalFormat:
            m_format_btn->setDefaultAction(m_set_octmode_act);
            m_view->setValueCoding(Okteta::AbstractByteArrayView::OctalCoding);
            break;
        case BinaryFormat:
            m_format_btn->setDefaultAction(m_set_binmode_act);
            m_view->setValueCoding(Okteta::AbstractByteArrayView::BinaryCoding);
            break;
        default:
            return;
    }

    m_format = format;
    m_values_encoder->setValueCoding(static_cast<Okteta::ValueCoding>(m_view->valueCoding()));
}

BytecodeEditor::DisplayFormat BytecodeEditor::displayFormat() const
{
    return m_format;
}

void BytecodeEditor::setDisplayMode(DisplayMode mode)
{
    if (mode == DisplayMode::CodePlusTextMode) {
        m_mode_btn->setDefaultAction(m_set_code_text_act);
        m_view->setVisibleCodings(Okteta::AbstractByteArrayView::ValueAndCharCodings);
    }
    else if (mode == DisplayMode::CodeOnlyMode) {
        m_mode_btn->setDefaultAction(m_set_code_only_act);
        m_view->setVisibleCodings(Okteta::AbstractByteArrayView::OnlyValueCoding);
    }
    else {
        m_mode_btn->setDefaultAction(m_set_text_only_act);
        m_view->setVisibleCodings(Okteta::AbstractByteArrayView::OnlyCharCoding);
    }

    m_mode = mode;
}

void BytecodeEditor::setColumnWidth(ColumnWidth width)
{
    if (width == ColumnWidth::One) {
        m_width_btn->setDefaultAction(m_set_width_1_act);
        m_view->setNoOfGroupedBytes(1);
    }
    else if (width == ColumnWidth::Two) {
        m_width_btn->setDefaultAction(m_set_width_2_act);
        m_view->setNoOfGroupedBytes(2);
    }
    else if (width == ColumnWidth::Four) {
        m_width_btn->setDefaultAction(m_set_width_4_act);
        m_view->setNoOfGroupedBytes(4);
    }
    else {
        m_width_btn->setDefaultAction(m_set_width_8_act);
        m_view->setNoOfGroupedBytes(8);
    }

    m_width = width;
}

BytecodeEditor::ColumnWidth BytecodeEditor::columnWidth() const
{
    return m_width;
}

BytecodeEditor::DisplayMode BytecodeEditor::displayMode() const
{
    return m_mode;
}

void BytecodeEditor::setData(const QByteArray& data)
{
    m_model->setData(data);
}

QByteArray BytecodeEditor::currentData() const
{
    QByteArray ret;
        ret.resize(m_model->size());
        m_model->copyTo(reinterpret_cast<Okteta::Byte*>(ret.data()),
                        Okteta::AddressRange(0, m_model->size()-1));

    return ret;
}

void BytecodeEditor::createActions()
{
    m_set_hexmode_act = new QAction(this);
        m_set_hexmode_act->setIcon(QIcon(":/icons/hex.svg"));
        m_set_hexmode_act->setText(QObject::tr("Hexademical"));

    m_set_decmode_act = new QAction(this);
        m_set_decmode_act->setIcon(QIcon(":/icons/dec.svg"));
        m_set_decmode_act->setText(QObject::tr("Decimal"));

    m_set_octmode_act = new QAction(this);
        m_set_octmode_act->setIcon(QIcon(":/icons/octal.svg"));
        m_set_octmode_act->setText(QObject::tr("Octal"));

    m_set_binmode_act = new QAction(this);
        m_set_binmode_act->setIcon(QIcon(":/icons/binary.svg"));
        m_set_binmode_act->setText(QObject::tr("Binary"));

    m_set_code_text_act = new QAction(this);
        m_set_code_text_act->setIcon(QIcon(":/icons/code_text.svg"));
        m_set_code_text_act->setText(QObject::tr("Code + text"));

    m_set_code_only_act = new QAction(this);
        m_set_code_only_act->setIcon(QIcon(":/icons/code.svg"));
        m_set_code_only_act->setText(QObject::tr("Code only"));

    m_set_text_only_act = new QAction(this);
        m_set_text_only_act->setIcon(QIcon(":/icons/ascii.svg"));
        m_set_text_only_act->setText(QObject::tr("Text only"));

    m_set_width_1_act = new QAction(this);
        m_set_width_1_act->setIcon(QIcon(":/icons/one.svg"));
        m_set_width_1_act->setText(QObject::tr("Byte"));

    m_set_width_2_act = new QAction(this);
        m_set_width_2_act->setIcon(QIcon(":/icons/two.svg"));
        m_set_width_2_act->setText(QObject::tr("Half word"));

    m_set_width_4_act = new QAction(this);
        m_set_width_4_act->setIcon(QIcon(":/icons/four.svg"));
        m_set_width_4_act->setText(QObject::tr("Word"));

    m_set_width_8_act = new QAction(this);
        m_set_width_8_act->setIcon(QIcon(":/icons/eight.svg"));
        m_set_width_8_act->setText(QObject::tr("Double word"));

    m_encode_as_chars_act  = new QAction(tr("Chars"), this);
    m_encode_as_values_act = new QAction(tr("Values"), this);
    m_encode_as_code_act   = new QAction(tr("C-array..."), this);

    m_copy_as_menu = new QMenu(tr("Copy as..."),this);
        m_copy_as_menu->addAction(m_encode_as_chars_act);
        m_copy_as_menu->addAction(m_encode_as_values_act);
        m_copy_as_menu->addAction(m_encode_as_code_act);
}

void BytecodeEditor::createEncoders()
{
    m_values_encoder = new BytecodeValuesEncoder(this);
        m_values_encoder->setValueCoding(static_cast<Okteta::ValueCoding>(m_view->valueCoding()));

    m_chars_encoder = new BytecodeCharsEncoder(this);
        m_chars_encoder->setCodecName(m_view->charCodingName());
        m_chars_encoder->setUndefinedChar(m_view->undefinedChar());
        m_chars_encoder->setSubstituteChar(m_view->substituteChar());

    m_code_encoder = new BytecodeSourceCodeEncoder(this);

    m_copy_as_dialog->setEncoder(m_code_encoder, new BytecodeSourceCodeEncoderConfigDialog(this));
}

void BytecodeEditor::createGui()
{
    m_copy_as_dialog = new BytecodeCopyAsDialog(this);

    m_view = new Okteta::ByteArrayColumnView(this);
        m_view->setByteArrayModel(m_model);
        m_view->setCharCoding("US-ASCII");
        m_view->setFrameShadow(QFrame::Raised);
        m_view->setNoOfGroupedBytes(4);
        m_view->setLayoutStyle(Okteta::AbstractByteArrayView::WrapOnlyByteGroupsLayoutStyle);
        m_view->setVisibleCodings(Okteta::AbstractByteArrayView::ValueAndCharCodings);
        m_view->setValueCoding(Okteta::AbstractByteArrayView::HexadecimalCoding);
        m_view->setOverwriteMode(false);
        m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    m_counter_label = new QLabel("0/0");

    m_codecs = new QComboBox();
        m_codecs->addItems(Okteta::CharCodec::codecNames());
        m_codecs->setCurrentText(m_view->charCodingName());
        m_codecs->setMaxVisibleItems(10);
        m_codecs->setStyleSheet("combobox-popup: 0;");

    auto menu_style = new MenuProxyStyle(QApplication::style());

    auto format_menu = new QMenu(this);
        format_menu->addAction(m_set_hexmode_act);
        format_menu->addAction(m_set_decmode_act);
        format_menu->addAction(m_set_octmode_act);
        format_menu->addAction(m_set_binmode_act);
        format_menu->setStyle(menu_style);

    m_format_btn = new QToolButton();
        m_format_btn->setDefaultAction(m_set_hexmode_act);
        m_format_btn->setMenu(format_menu);
        m_format_btn->setIconSize({18, 18});

    auto mode_menu = new QMenu(this);
        mode_menu->addAction(m_set_code_text_act);
        mode_menu->addAction(m_set_code_only_act);
        mode_menu->addAction(m_set_text_only_act);
        mode_menu->setStyle(menu_style);

    m_mode_btn = new QToolButton();
        m_mode_btn->setDefaultAction(m_set_code_text_act);
        m_mode_btn->setMenu(mode_menu);
        m_mode_btn->setIconSize({18, 18});

    auto width_menu = new QMenu(this);
        width_menu->addAction(m_set_width_1_act);
        width_menu->addAction(m_set_width_2_act);
        width_menu->addAction(m_set_width_4_act);
        width_menu->addAction(m_set_width_8_act);
        width_menu->setStyle(menu_style);

    m_width_btn = new QToolButton();
        m_width_btn->setDefaultAction(m_set_width_4_act);
        m_width_btn->setMenu(width_menu);
        m_width_btn->setIconSize({18, 18});

    auto horizontalLayout = new QHBoxLayout();
        horizontalLayout->addWidget(m_format_btn);
        horizontalLayout->addWidget(m_mode_btn);
        horizontalLayout->addWidget(m_width_btn);
        horizontalLayout->addWidget(m_codecs);
        horizontalLayout->addStretch();
        horizontalLayout->addWidget(m_counter_label);


    auto verticalLayout = new QVBoxLayout();
        verticalLayout->addLayout(horizontalLayout);
        verticalLayout->addWidget(m_view);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(verticalLayout);
}

void BytecodeEditor::connectSignals()
{
    connect(m_codecs, &QComboBox::currentTextChanged, this, [this](const QString& name) {
        m_view->setCharCoding(name);
        m_chars_encoder->setCodecName(name);
    });

    connect(m_mode_btn, &QToolButton::clicked, this, [this]() {
        if (m_mode == CodePlusTextMode) {
            setDisplayMode(CodeOnlyMode);
        }
        else if (m_mode == CodeOnlyMode) {
            setDisplayMode(TextOnlyMode);
        }
        else {
            setDisplayMode(CodePlusTextMode);
        }
    });

    connect(m_set_code_text_act, &QAction::triggered, this, [this]() {
        setDisplayMode(CodePlusTextMode);
    });

    connect(m_set_code_only_act, &QAction::triggered, this, [this]() {
        setDisplayMode(CodeOnlyMode);
    });

    connect(m_set_text_only_act, &QAction::triggered, this, [this]() {
        setDisplayMode(TextOnlyMode);
    });

    connect(m_format_btn, &QToolButton::clicked, this, [this]() {
        switch(m_format) {
            case HexFormat:
                setDisplayFormat(DecimalFormat);
                break;
            case DecimalFormat:
                setDisplayFormat(OctalFormat);
                break;
            case OctalFormat:
                setDisplayFormat(BinaryFormat);
                break;
            case BinaryFormat:
                setDisplayFormat(HexFormat);
                break;
            default:
                return;
            }
    });

    connect(m_set_hexmode_act, &QAction::triggered, this, [this]() {
        setDisplayFormat(HexFormat);
    });

    connect(m_set_decmode_act, &QAction::triggered, this, [this]() {
        setDisplayFormat(DecimalFormat);
    });

    connect(m_set_octmode_act, &QAction::triggered, this, [this]() {
        setDisplayFormat(OctalFormat);
    });

    connect(m_set_binmode_act, &QAction::triggered, this, [this]() {
        setDisplayFormat(BinaryFormat);
    });


    connect(m_width_btn, &QToolButton::clicked, this, [this]() {
        switch(m_width) {
        case ColumnWidth::One:
            setColumnWidth(ColumnWidth::Two);
            break;
        case ColumnWidth::Two:
            setColumnWidth(ColumnWidth::Four);
            break;
        case ColumnWidth::Four:
            setColumnWidth(ColumnWidth::Eight);
            break;
        case ColumnWidth::Eight:
            setColumnWidth(ColumnWidth::One);
            break;
        default:
            return;
        }
    });

    connect(m_set_width_1_act, &QAction::triggered, this, [this]() {
        setColumnWidth(ColumnWidth::One);
    });

    connect(m_set_width_2_act, &QAction::triggered, this, [this]() {
        setColumnWidth(ColumnWidth::Two);
    });

    connect(m_set_width_4_act, &QAction::triggered, this, [this]() {
        setColumnWidth(ColumnWidth::Four);
    });

    connect(m_set_width_8_act, &QAction::triggered, this, [this]() {
        setColumnWidth(ColumnWidth::Eight);
    });

    connect(m_view, &Okteta::ByteArrayColumnView::cursorPositionChanged, this, [this](Okteta::Address index) {
        m_counter_label->setText(QString("%1/%2")
            .arg(QString::number(m_view->cursorPosition()), QString::number(m_model->size())));
    });

    connect(m_encode_as_chars_act , &QAction::triggered, this, [this]() {
        auto text = m_chars_encoder->encodeData(m_view->selectedData());
        QGuiApplication::clipboard()->setText(text);
    });

    connect(m_encode_as_values_act, &QAction::triggered, this, [this]() {
        auto text = m_values_encoder->encodeData(m_view->selectedData());
        QGuiApplication::clipboard()->setText(text);
    });

    connect(m_encode_as_code_act, &QAction::triggered, this, [this]() {
        m_copy_as_dialog->setData(m_view->selectedData());
        m_copy_as_dialog->show();
    });

    connect(m_view, &QWidget::customContextMenuRequested, this, [this](const QPoint pos) {
        auto menu = m_view->createStandardContextMenu(pos);
        if (menu) {
            auto actions = menu->actions();
            for (int i = 0; i < actions.size(); i++) {
                if (actions.at(i)->text().contains("Copy")) {
                    menu->insertMenu(actions.at(i), m_copy_as_menu);
                    break;
                }

                if (i == actions.size() - 1) {
                    menu->addMenu(m_copy_as_menu);
                }
            }

            m_copy_as_menu->setEnabled(m_view->hasSelectedData());
            menu->popup(mapToGlobal(pos));
        }
    });
}
