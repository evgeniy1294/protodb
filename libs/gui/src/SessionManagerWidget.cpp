#include <protodb/gui/SessionManagerWidget.h>

#include <QTableView>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QHeaderView>

SessionManagerDialog::SessionManagerDialog(QWidget *parent)
    : QDialog(parent)
{
    create_gui();
    create_connections();

    resize(640, 480);
    setWindowTitle(tr("Sessions"));
}

void SessionManagerDialog::create_gui()
{
    m_dialog_buttons = new QDialogButtonBox( QDialogButtonBox::Close );

    m_sessions_table = new QTableView;

    m_restore_chk = new QCheckBox(tr("Restore last session"));
    m_create_btn = new QPushButton(tr("Create"));
    m_rename_btn = new QPushButton(tr("Rename"));
    m_copy_btn = new QPushButton(tr("Copy"));
    m_del_btn = new QPushButton(tr("Delete"));
    m_select_btn = new QPushButton(tr("Select"));

    auto btn_layout = new QVBoxLayout;
        btn_layout->setAlignment(Qt::AlignTop);
        btn_layout->addWidget(m_create_btn);
        btn_layout->addWidget(m_rename_btn);
        btn_layout->addWidget(m_copy_btn);
        btn_layout->addWidget(m_del_btn);
        btn_layout->addWidget(m_select_btn);

    auto stretch_layout = new QVBoxLayout;
        stretch_layout->addWidget(m_sessions_table);
        stretch_layout->addWidget(m_restore_chk);

    auto main_layout = new QGridLayout;
        main_layout->addLayout(stretch_layout, 0, 0, Qt::AlignLeft);
        main_layout->addLayout(btn_layout, 0, 1, Qt::AlignTop);
        main_layout->addWidget(m_dialog_buttons, 1, 0, 1, 2);

    setLayout(main_layout);
}

void SessionManagerDialog::create_connections()
{
    connect(m_dialog_buttons, &QDialogButtonBox::clicked, this, &SessionManagerDialog::onDialogClicked);
}


void SessionManagerDialog::onDialogClicked(QAbstractButton* aBtn)
{
    switch( m_dialog_buttons->standardButton( aBtn ) )
    {
        case QDialogButtonBox::Close:
            hide();
            break;

        default:
            break;
    }
}

void SessionManagerDialog::setSessionManager(SessionManager *sm)
{
    if (m_sm != sm) {
        m_sm = sm;
        m_sessions_table->setModel( sm );
        m_sessions_table->hideColumn(SessionManager::kColumnDescription);

        QHeaderView* hh = m_sessions_table->horizontalHeader();
            hh->setSectionResizeMode(SessionManager::kColumnName, QHeaderView::Stretch);
            hh->setSectionResizeMode(SessionManager::kColumnLastChanged, QHeaderView::Stretch);
    }
}

SessionManager *SessionManagerDialog::sessionManager() const
{
    return m_sm;
}
