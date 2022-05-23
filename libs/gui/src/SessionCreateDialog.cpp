#include <protodb/gui/SessionCreateDialog.h>
#include <protodb/SessionManager.h>

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QLabel>

SessionCreateDialog::SessionCreateDialog(SessionManager* sm, QWidget *parent)
    : QDialog(parent)
    , m_sm(sm)
    , m_create_mode(true)
{
    create_gui();
    create_connections();
}

void SessionCreateDialog::setCreateMode(bool on)
{
    m_create_mode = on;

    if (on)
        setWindowTitle(tr("Create session"));
    else
        setWindowTitle(tr("Change session"));
}

bool SessionCreateDialog::isCreateMode() const
{
    return m_create_mode;
}

void SessionCreateDialog::setChangeMode(bool on)
{
    m_create_mode = !on;

    if (on)
        setWindowTitle(tr("Change session"));
    else
        setWindowTitle(tr("Create session"));
}

bool SessionCreateDialog::isChangeMode() const
{
    return !m_create_mode;
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

    m_error_label = new QLabel("");
        m_error_label->setStyleSheet("QLabel { color: red }");

    auto main_layout = new QVBoxLayout();
        main_layout->addWidget(m_name);
        main_layout->addWidget(m_desc);
        main_layout->addWidget(m_error_label);
        main_layout->addWidget(m_dialog_buttons);

    setLayout(main_layout);

    resize(480, 240);
    setModal(true);
    setWindowTitle(tr("Create session"));
}

void SessionCreateDialog::create_connections()
{
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &SessionCreateDialog::onDialogClicked);
    connect(m_name, &QLineEdit::textChanged, this, &SessionCreateDialog::onNameTextChanged);
}

void SessionCreateDialog::onDialogClicked(QAbstractButton* aBtn)
{
    switch( m_dialog_buttons->standardButton( aBtn ) )
    {
        case QDialogButtonBox::Close:
            setResult(QDialog::Rejected);
            close();
            break;

        case QDialogButtonBox::Ok:
            if ( m_sm->containsSession(m_name->text()) ) {
                m_error_label->setText( tr("Session with that name is already created") );
                break;
            }

            if ( m_sm->createSession( m_name->text(), m_desc->toPlainText() ) ) {
                m_error_label->setText("");
                setResult(QDialog::Accepted);
                close();
            }
            else {
                m_error_label->setText( m_sm->lastError() );
            }

        default:
            break;
    }
}

void SessionCreateDialog::onNameTextChanged(const QString &text)
{
    if (m_sm->containsSession(text)) {
        m_name->setStyleSheet("QLineEdit { background-color: #FFCECE }");
    }
    else {
        m_name->setStyleSheet("QLineEdit { background-color: white }");
    }
}

