#include "SequenceEditor.h"

#include <Okteta/CharCodec.hpp>
#include <Okteta/ByteArrayColumnView.hpp>
#include <Okteta/PieceTableByteArrayModel.hpp>

#include <QApplication>
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


SequenceEditor::SequenceEditor(QWidget* parent)
    : QWidget(parent)
    , m_format(HexFormat)
    , m_mode(CodePlusTextMode)
{
    m_model = new Okteta::PieceTableByteArrayModel(QByteArray(), this);

    createActions();
    createGui();
    connectSignals();
}

void SequenceEditor::setDisplayFormat(DisplayFormat format)
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
}

SequenceEditor::DisplayFormat SequenceEditor::displayFormat() const
{
    return m_format;
}

void SequenceEditor::setDisplayMode(DisplayMode mode)
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

SequenceEditor::DisplayMode SequenceEditor::displayMode() const
{
    return m_mode;
}

void SequenceEditor::setData(const QByteArray& data)
{
    m_model->setData(data);
}

QByteArray SequenceEditor::currentData() const
{
    return QByteArray();
}

void SequenceEditor::createActions()
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
}

void SequenceEditor::createGui()
{
    m_view = new Okteta::ByteArrayColumnView(this);
        m_view->setByteArrayModel(m_model);
        m_view->setFrameShadow(QFrame::Raised);
        m_view->setVisibleCodings(4);
        m_view->setLayoutStyle(Okteta::AbstractByteArrayView::WrapOnlyByteGroupsLayoutStyle);
        m_view->setVisibleCodings(Okteta::AbstractByteArrayView::ValueAndCharCodings);
        m_view->setValueCoding(Okteta::AbstractByteArrayView::HexadecimalCoding);
        m_view->setOverwriteMode(false);

    m_codecs = new QComboBox();
        m_codecs->addItems(Okteta::CharCodec::codecNames());

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

    auto horizontalLayout = new QHBoxLayout();
        horizontalLayout->addWidget(m_format_btn);
        horizontalLayout->addWidget(m_mode_btn);
        horizontalLayout->addWidget(m_codecs);
        horizontalLayout->addStretch();


    auto verticalLayout = new QVBoxLayout();
        verticalLayout->addLayout(horizontalLayout);
        verticalLayout->addWidget(m_view);
        verticalLayout->setMargin(0);

    setLayout(verticalLayout);
}

void SequenceEditor::connectSignals()
{
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
}
