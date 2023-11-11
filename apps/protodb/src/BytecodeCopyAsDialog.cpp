#include "BytecodeCopyAsDialog.h"

#include <protodb/ConfigWidgetAbstract.h>
#include <protodb/BytecodeEncoderAbstract.h>
#include <protodb/creators/BytecodeEncoderCreator.h>
#include <protodb/factories/BytecodeEncoderFactory.h>

#include <nlohmann/json.hpp>

#include <QAbstractButton>
#include <QComboBox>
#include <QClipboard>
#include <QDialogButtonBox>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QGuiApplication>

using namespace protodb;

BytecodeCopyAsDialog::BytecodeCopyAsDialog(QWidget* parent)
    : QDialog(parent)
{
    createGui();
    connectSignals();

    resize(720, 310);
}

void BytecodeCopyAsDialog::createGui()
{
    m_dialog_btn   = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_preview      = new QPlainTextEdit();
    m_dummy_wgt    = new QWidget();
    m_cofig_widget = nullptr;

    m_layout = new QGridLayout();
        m_layout->addWidget(m_dummy_wgt, 0, 0, 1, 1, Qt::AlignTop);
        m_layout->addWidget(m_preview, 0, 1, 1, 1);
        m_layout->addWidget(m_dialog_btn, 2, 0, 1, 2, Qt::AlignRight);
        m_layout->setColumnStretch(0, 2);
        m_layout->setColumnStretch(1, 5);

    setLayout(m_layout);
    setWindowTitle(tr("Copy as..."));
}

void BytecodeCopyAsDialog::connectSignals()
{
    connect(m_dialog_btn, &QDialogButtonBox::clicked, this, &BytecodeCopyAsDialog::onDialogClicked);
}

void BytecodeCopyAsDialog::onDialogClicked(QAbstractButton* btn)
{
    switch( m_dialog_btn->standardButton( btn ) )
    {
        case QDialogButtonBox::Ok: {
            QGuiApplication::clipboard()->setText( m_preview->toPlainText() );
            accept();
        } break;

        case QDialogButtonBox::Cancel:
            hide();
            break;

        default:
            break;
        }
}

void BytecodeCopyAsDialog::updatePreview()
{
    if (m_encoder == nullptr || m_cofig_widget == nullptr)
        return;

    nlohmann::json cfg;
        m_cofig_widget->config(cfg);
        m_encoder->setConfig(cfg);
        m_preview->setPlainText(m_encoder->encodeData(m_data));
}

void BytecodeCopyAsDialog::setEncoder(BytecodeEncoderAbstract* encoder, ConfigWidgetAbstract* widget)
{
    if (encoder == nullptr || widget == nullptr)
        return;

    if (m_encoder != encoder) {
        if (m_cofig_widget != nullptr) {
            disconnect(m_cofig_widget);
        }

        m_encoder = encoder;
        m_cofig_widget = widget;
        m_cofig_widget->layout()->setContentsMargins(0, 0, 0, 0);
        m_layout->replaceWidget(m_dummy_wgt, m_cofig_widget);

        nlohmann::json cfg;
            m_encoder->config(cfg);
            m_cofig_widget->setConfig(cfg);
            m_preview->setPlainText(m_encoder->encodeData(m_data));

        connect(m_cofig_widget, &ConfigWidgetAbstract::sConfigChanged, this, &BytecodeCopyAsDialog::updatePreview);
    }
}

void BytecodeCopyAsDialog::setData(const QByteArray& data)
{
    m_data = data;
    updatePreview();
}

