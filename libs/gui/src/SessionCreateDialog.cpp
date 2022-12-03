#include "protodb/sessions/SessionCreateDialog.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QLabel>

using namespace protodb;

SessionCreateDialog::SessionCreateDialog(QWidget *parent)
    : QDialog(parent)
    , m_mode(CreateMode)
{
    create_gui();
    create_connections();
}

void SessionCreateDialog::setMode(Mode mode)
{
    m_mode = mode;

    if (m_mode == CreateMode)
        setWindowTitle(tr("Create session"));
    else
        setWindowTitle(tr("Edit session"));
}

bool SessionCreateDialog::isCreateMode() const
{
    return m_mode == CreateMode;
}

bool SessionCreateDialog::isEditMode() const
{
    return m_mode == EditMode;
}

void SessionCreateDialog::setSessionName(const QString& name)
{
    m_name->setText(name);
}

QString SessionCreateDialog::sessionName() const
{
    return m_name->text();
}

void SessionCreateDialog::setSessionDescription(const QString& desc)
{
    m_desc->setPlainText(desc);
}

QString SessionCreateDialog::sessionDescription() const
{
    return m_desc->toPlainText();
}

void SessionCreateDialog::create_gui()
{
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Close );

    m_name = new QLineEdit();
        m_name->setToolTip( tr("Session name") );
        m_name->setPlaceholderText( tr("Session name") );

    m_desc = new QPlainTextEdit;
        m_desc->setToolTip( tr("Session description") );
        m_desc->setPlaceholderText( tr("Document me!") );

    auto main_layout = new QVBoxLayout();
        main_layout->addWidget(m_name);
        main_layout->addWidget(m_desc);
        main_layout->addWidget(m_dialog_buttons);

    setLayout(main_layout);

    resize(480, 240);
    setModal(true);
    setWindowTitle(tr("Create session"));
}

void SessionCreateDialog::create_connections()
{
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &SessionCreateDialog::onDialogClicked);
}

void SessionCreateDialog::onDialogClicked(QAbstractButton* aBtn)
{
    switch( m_dialog_buttons->standardButton( aBtn ) )
    {
        case QDialogButtonBox::Close:
            reject();
            break;

        case QDialogButtonBox::Ok: {
            accept();
        } break;

        default:
            break;
    }
}

