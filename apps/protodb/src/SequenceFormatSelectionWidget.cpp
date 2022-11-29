#include "SequenceFormatSelectionWidget.h"
#include "Sequence.h"

#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>

SequenceFormatSelectionWidget::SequenceFormatSelectionWidget(QObject* parent)
{
    create_gui();
    connect_signals();

    m_btn_group->setExclusive(true);
    setFormatSelection(Sequence::DefaultFormatId);
}

void SequenceFormatSelectionWidget::create_gui()
{
    auto m_layout = new QHBoxLayout;
    m_btn_group   = new QButtonGroup(this);

    for (auto& format: Sequence::supportedFormats()) {
        auto btn = new QRadioButton(format);
            m_btn_group->addButton(btn);
            m_layout->addWidget(btn);
    }

    m_layout->setAlignment(Qt::AlignLeft);
    setLayout(m_layout);
}

void SequenceFormatSelectionWidget::connect_signals()
{
    connect(m_btn_group, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [this](QAbstractButton* btn, bool checked) {
        if (checked) {
            m_selected_format = btn->text();
            emit selectedFormatChanged(m_selected_format);
        }
    });
}

QString SequenceFormatSelectionWidget::selectedFormat() const
{
    return m_selected_format;
}

void SequenceFormatSelectionWidget::setFormatSelection(const QString& fmt)
{
    if (fmt == m_selected_format)
        return;

    if (Sequence::supportedFormats().contains(fmt)) {
        blockSignals(true);

        auto buttons = m_btn_group->buttons();
        for(int i = 0; i < buttons.count(); i++) {
            auto btn = buttons[i]; if (btn->text() == fmt) {
                btn->setChecked(true);
                m_selected_format = fmt;

                emit selectedFormatChanged(fmt);
            }
        }

        blockSignals(false);
    }

    return;
}
