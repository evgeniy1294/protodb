#include <protodb/gui/SessionManagerGui.h>
#include <protodb/gui/SessionCreateDialog.h>

#include <QTableView>
#include <QTextBrowser>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

SessionManagerGui::SessionManagerGui(QWidget *parent)
    : QDialog(parent)
{
    create_gui();
    create_connections();
    disable_control_btn();
}

void SessionManagerGui::create_gui()
{
    m_proxy_model = new QSortFilterProxyModel();

    m_sessions_table = new QTableView;
        m_sessions_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_sessions_table->setShowGrid(false);
        m_sessions_table->setModel( m_proxy_model );

    m_filter_le = new QLineEdit();
        m_filter_le->setPlaceholderText( tr("Find session") );

    m_desc_browser   = new QTextBrowser();
    m_close_btn = new QPushButton( tr("Close") );
    m_create_btn = new QPushButton(tr("Create"));
    m_change_btn = new QPushButton(tr("Edit"));
    m_copy_btn = new QPushButton(tr("Copy"));
    m_rm_btn = new QPushButton(tr("Delete"));
    m_select_btn = new QPushButton(tr("Switch to"));
    m_import_btn = new QPushButton(tr("Import"));
    m_export_btn = new QPushButton(tr("Export"));

    auto btn_layout = new QVBoxLayout;
        btn_layout->setAlignment(Qt::AlignTop);
        btn_layout->addWidget(m_select_btn);
        btn_layout->addWidget(m_create_btn);
        btn_layout->addWidget(m_import_btn);
        btn_layout->addWidget(m_export_btn);
        btn_layout->addWidget(m_copy_btn);
        btn_layout->addWidget(m_change_btn);
        btn_layout->addWidget(m_rm_btn);
        btn_layout->addStretch(1);
        btn_layout->addWidget(m_close_btn);

    auto stretch_layout = new QVBoxLayout;
        stretch_layout->addWidget(m_filter_le);
            stretch_layout->setStretch(0, 0);
        stretch_layout->addWidget(m_sessions_table);
            stretch_layout->setStretch(1, 4);
        stretch_layout->addWidget(m_desc_browser);
            stretch_layout->setStretch(2, 1);

    auto main_layout = new QGridLayout;
        main_layout->addLayout(stretch_layout, 0, 0, Qt::AlignLeft);
        main_layout->addLayout(btn_layout, 0, 1, Qt::AlignTop);

    setLayout(main_layout);

    resize(640, 480);
    setWindowTitle(tr("Sessions"));
}

void SessionManagerGui::create_connections()
{
    connect(m_create_btn, &QPushButton::clicked, this, &SessionManagerGui::onCreateClicked);
    connect(m_change_btn, &QPushButton::clicked, this, &SessionManagerGui::onChangeClicked);
    connect(m_rm_btn, &QPushButton::clicked, this, &SessionManagerGui::onRmClicked);
    connect(m_filter_le, &QLineEdit::textChanged, m_proxy_model, &QSortFilterProxyModel::setFilterFixedString);
    connect(m_export_btn, &QPushButton::clicked, this, &SessionManagerGui::onExportClicked);
    connect(m_import_btn, &QPushButton::clicked, this, &SessionManagerGui::onImportClicked);
    connect(m_close_btn, &QPushButton::clicked, this, [this]() {
        hide();
    });

    connect(m_sessions_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SessionManagerGui::onSelectionChanged);
}

void SessionManagerGui::disable_control_btn()
{
    m_change_btn->setDisabled(true);
    m_copy_btn->setDisabled(true);
    m_rm_btn->setDisabled(true);
    m_select_btn->setDisabled(true);
    m_export_btn->setDisabled(true);
}

void SessionManagerGui::enable_control_btn()
{
    m_change_btn->setEnabled(true);
    m_copy_btn->setEnabled(true);
    m_rm_btn->setEnabled(true);
    m_select_btn->setEnabled(true);
    m_export_btn->setEnabled(true);
}

void SessionManagerGui::onCreateClicked()
{
    SessionCreateDialog dialog;
        dialog.setSessionName(tr("New session"));

    if (dialog.exec() == QDialog::Accepted) {
        if (m_sm->createSession(dialog.sessionName(), dialog.sessionDescription()).isEmpty()) {
            QMessageBox mbox;
                mbox.setText("Error");
                mbox.setIcon(QMessageBox::Critical);
                mbox.setInformativeText(m_sm->lastError());
                mbox.setStandardButtons(QMessageBox::Ok);

            mbox.exec();
        }
    }

    return;
}

void SessionManagerGui::onChangeClicked()
{
    auto idx = m_proxy_model->mapToSource( m_sessions_table->selectionModel()->selectedRows().first() ).row();

    QString old_name = m_sm->data( m_sm->index(idx, SessionManager::kColumnName) ).toString();
    QString old_desc = m_sm->data( m_sm->index(idx, SessionManager::kColumnDescription) ).toString();

    SessionCreateDialog dialog;
    {
        dialog.setMode( SessionCreateDialog::EditMode );
        dialog.setSessionName( old_name );
        dialog.setSessionDescription(old_desc);
    }

    if (dialog.exec() == QDialog::Accepted) {
        if (old_name != dialog.sessionName()) {
            if (!m_sm->renameSession(idx, dialog.sessionName())) {
                QMessageBox mbox;
                    mbox.setText("Error");
                    mbox.setIcon(QMessageBox::Critical);
                    mbox.setInformativeText(m_sm->lastError());
                    mbox.setStandardButtons(QMessageBox::Ok);

                mbox.exec();
            }
        }

        if (old_desc != dialog.sessionDescription()) {
            if ( m_sm->setData(m_sm->index(idx, SessionManager::kColumnDescription), dialog.sessionDescription(), Qt::EditRole) ) {
                m_desc_browser->setPlainText(dialog.sessionDescription());
            }
        }
    }

    return;
}

void SessionManagerGui::onRmClicked()
{
    auto selected = m_sessions_table->selectionModel()->selectedRows();
    if (selected.empty())
        return;

    QString name = selected.first().data().toString();

    if ( name.isEmpty() )
        return;

    QMessageBox msgbox;
    {
        msgbox.setText( tr("Delete %1 session?").arg(name) );
        msgbox.setInformativeText( tr("Selected session will be removed") );
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setDefaultButton(QMessageBox::No);
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setFixedSize( QSize(480, 240) );

            msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
            msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
    }

    if (msgbox.exec() == QMessageBox::Yes) {
        if ( !m_sm->removeSession(name) ) {
            msgbox.setStandardButtons(QMessageBox::Yes);
            msgbox.setText( tr("Error") );
            msgbox.setInformativeText( tr("Can't remove selected session") );

            msgbox.exec();
        }
    }
}

void SessionManagerGui::onExportClicked()
{
    auto selected = m_sessions_table->selectionModel()->selectedRows();
    if (selected.empty()) {
        return;
    }

    QString name = selected.first().data().toString();
    if ( name.isEmpty() ) {
        return;
    }

    QString exportPath = QDir::homePath() + "/" + name + ".zip";
    exportPath = QFileDialog::getSaveFileName(this,
        tr("Select session"), exportPath, tr("Session files (*.zip)"));

    if (exportPath.isEmpty()) {
        return;
    }

    if ( !m_sm->exportSession(m_proxy_model->mapToSource(selected.first()).row(), exportPath) ) {
        QMessageBox mbox;
            mbox.setText("Export error");
            mbox.setIcon(QMessageBox::Critical);
            mbox.setInformativeText(m_sm->lastError());
            mbox.setStandardButtons(QMessageBox::Ok);

        mbox.exec();
    }
}

void SessionManagerGui::onImportClicked()
{
    QString importPath = QDir::homePath();
    importPath = QFileDialog::getOpenFileName(this,
        tr("Select session"), importPath, tr("Session files (*.zip)"));

    if (importPath.isEmpty()) {
        return;
    }

    if ( !m_sm->importSession(importPath) ) {
        QMessageBox mbox;
            mbox.setText("Import error");
            mbox.setIcon(QMessageBox::Critical);
            mbox.setInformativeText(m_sm->lastError());
            mbox.setStandardButtons(QMessageBox::Ok);

        mbox.exec();
    }
}

void SessionManagerGui::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.count() > 0) {
        auto index = m_proxy_model->index(selected.indexes().first().row(), SessionManager::kColumnDescription);
        auto text  = index.data().toString();

        m_desc_browser->setPlainText(text);
        enable_control_btn();
    }
    else {
        m_desc_browser->clear();
        disable_control_btn();
    }
}

void SessionManagerGui::setSessionManager(SessionManager *sm)
{
    if (m_sm != sm) {
        m_sm = sm;

        m_proxy_model->setSourceModel(m_sm);
        m_sessions_table->hideColumn(SessionManager::kColumnDescription);

        QHeaderView* hh = m_sessions_table->horizontalHeader();
            hh->setSectionResizeMode(SessionManager::kColumnName, QHeaderView::Stretch);
            hh->setSectionResizeMode(SessionManager::kColumnLastChanged, QHeaderView::ResizeToContents);

        QHeaderView* vh = m_sessions_table->verticalHeader();
            vh->hide();
    }
}

SessionManager *SessionManagerGui::sessionManager() const
{
    return m_sm;
}


