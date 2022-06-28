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
    , m_mode(CreateMode)
{
    create_gui();
    create_connections();
}

void SessionCreateDialog::setSessionIndex(int idx)
{
    m_session_idx = idx;

    if ( !m_mode ) {
        auto name = m_sm->index(idx, SessionManager::kColumnName).data().toString();
            m_name->setText(name);

        auto desc = m_sm->index(idx, SessionManager::kColumnDescription).data().toString();
            m_desc->setPlainText(desc);
    }
}

int SessionCreateDialog::sessionIndex() const
{
    return m_session_idx;
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

        case QDialogButtonBox::Ok: {
            if (m_mode == CreateMode) {
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
            }
            else {
                // Редактирование сессии
            }
        } break;

        default:
            break;
    }
}

void SessionCreateDialog::onNameTextChanged(const QString &text)
{
    auto idx = m_sm->findSessionByName(text);
    if (idx >= 0) {
        if (m_session_idx != idx) {
            m_name->setStyleSheet("QLineEdit { background-color: #FFCECE }");
        }
    }
    else {
        m_name->setStyleSheet("QLineEdit { background-color: white }");
    }
}

