#include "SequenceModel.h"
#include "SequenceEditDialog.h"
#include "SequenceFormatSelectionWidget.h"
#include "SequenceEditor.h"

#include <QLineEdit>
#include <QTextBrowser>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QDataWidgetMapper>
#include <QAbstractItemModel>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QMessageBox>

using namespace protodb;

SequenceEditDialog::SequenceEditDialog(QWidget* parent)
    : QDialog(parent)
    , m_mapper(nullptr)
{
    createGui();
    createConnections();
}

QDataWidgetMapper* SequenceEditDialog::mapper() const
{
    return m_mapper;
}

void SequenceEditDialog::setMapper(QDataWidgetMapper* mapper)
{
    if (m_mapper != mapper) {
        if ( m_mapper != nullptr ) {
            m_mapper->clearMapping();
            disconnect(m_mapper);
        }

        m_mapper = mapper;
        m_mapper->addMapping(m_name_edit, SequenceModel::kColumnName);
        m_mapper->addMapping(m_desc_editor, SequenceModel::kColumnDescription);
        m_mapper->addMapping(m_dsl_editor, SequenceModel::kColumnDsl);
        m_mapper->addMapping(m_format_selection_wgt, SequenceModel::kColumnSyntaxId, "selectedFormat");

        auto onIndexChanged = [this](int idx) {
            auto model = qobject_cast<SequenceModel*>(m_mapper->model());
            if (model == nullptr) {
                return;
            }

            if (model->rowCount() > 0) {

                m_item_label->setText(QString("%1/%2").arg(idx+1).arg(m_mapper->model()->rowCount()));

                m_name_edit->setDisabled(false);
                m_desc_editor->setDisabled(false);
                m_dsl_editor->setDisabled(false);

                if (idx <= 0) {
                    m_back_btn->setDisabled(true);
                    m_prev_btn->setDisabled(true);
                }
                else
                {
                    m_back_btn->setDisabled(false);
                    m_prev_btn->setDisabled(false);
                }

                if (idx >= (m_mapper->model()->rowCount() - 1)) {
                    m_next_btn->setDisabled(true);
                    m_front_btn->setDisabled(true);
                }
                else
                {
                    m_next_btn->setDisabled(false);
                    m_front_btn->setDisabled(false);
                }
            }
        };

        connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, this, onIndexChanged);
        onIndexChanged(m_mapper->currentIndex());
    }
}

void SequenceEditDialog::reload()
{
    m_item_label->setText(QString("0/0"));
    m_name_edit->setDisabled(true);
    m_desc_editor->setDisabled(true);
    m_dsl_editor->setDisabled(true);
    m_back_btn->setDisabled(true);
    m_prev_btn->setDisabled(true);
    m_next_btn->setDisabled(true);
    m_front_btn->setDisabled(true);
}

void SequenceEditDialog::createGui()
{
    setWindowTitle(tr("Sequence Editor"));


    auto h_layout = new QHBoxLayout();

        auto format_group_box = new QGroupBox();
            m_format_selection_wgt = new SequenceFormatSelectionWidget();

            auto format_group_box_layout = new QHBoxLayout();
            format_group_box_layout->addWidget(m_format_selection_wgt);

            format_group_box->setLayout(format_group_box_layout);

        m_back_btn  = new QPushButton();
            m_back_btn->setIcon(QIcon(":/icons/arrow_back.svg"));

        m_prev_btn  = new QPushButton();
            m_prev_btn->setIcon(QIcon(":/icons/arrow_left.svg"));

        m_next_btn  = new QPushButton();
            m_next_btn->setIcon(QIcon(":/icons/arrow_right.svg"));

        m_front_btn = new QPushButton();
            m_front_btn->setIcon(QIcon(":/icons/arrow_front.svg"));

        m_add_btn   = new QPushButton();
            m_add_btn->setIcon(QIcon(":/icons/add.svg"));

        m_item_label = new QLabel(tr("0/0"));

        h_layout->addWidget(m_back_btn);
        h_layout->addWidget(m_prev_btn);
        h_layout->addWidget(m_item_label);
        h_layout->addWidget(m_next_btn);
        h_layout->addWidget(m_front_btn);
        h_layout->addStretch();
        h_layout->addWidget(m_add_btn);

    m_name_edit = new QLineEdit();
        m_name_edit->setPlaceholderText(tr("Sequence name"));

    m_desc_editor = new QPlainTextEdit();
        m_desc_editor->setPlaceholderText(tr("Document me!"));

    m_dsl_editor = new QPlainTextEdit();
        m_dsl_editor->setPlaceholderText(tr("CRC:Modbus{bytes}"));

    m_dialog_btn = new QDialogButtonBox( QDialogButtonBox::Ok|
                                       QDialogButtonBox::Apply|
                                       QDialogButtonBox::Reset|
                                       QDialogButtonBox::Cancel );

    m_editor = new SequenceEditor();

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(m_name_edit, 1, 0);
        main_layout->addWidget(m_desc_editor, 2, 0);
        // main_layout->addWidget(format_group_box, 3, 0);
        main_layout->addWidget(m_editor,  3, 0);
        //main_layout->addWidget(m_dsl_editor,  5, 0);
        main_layout->addWidget(m_dialog_btn,  4, 0);
        main_layout->setRowStretch(2, 2);
        main_layout->setRowStretch(3, 5);

    resize(680, 570);
    setLayout(main_layout);
    setWindowModality(Qt::NonModal);
}

void SequenceEditDialog::createConnections()
{
    connect(m_back_btn, &QPushButton::released, this, [this]() { if (m_mapper != nullptr) {
        if (checkUnsavedChanges()) {
            m_mapper->toFirst();
        }
    }});

    connect(m_prev_btn, &QPushButton::released, this, [this]() { if (m_mapper != nullptr) {
        if (checkUnsavedChanges()) {
            m_mapper->toPrevious();
        }
    }});

    connect(m_next_btn, &QPushButton::released, this, [this]() { if (m_mapper != nullptr) {
        if (checkUnsavedChanges()) {
            m_mapper->toNext();
        }
    }});

    connect(m_front_btn, &QPushButton::released, this, [this]() {if (m_mapper != nullptr) {
        if (checkUnsavedChanges()) {
            m_mapper->toLast();
        }
    }});

    connect(m_add_btn, &QPushButton::released, this, [this]() {
        if (m_mapper != nullptr) {
            if (checkUnsavedChanges()) {
                auto row = m_mapper->currentIndex() + 1;
                m_mapper->model()->insertRow(row);
                m_mapper->toNext();
            }
        }
    });

    connect(m_dialog_btn, &QDialogButtonBox::clicked, this, &SequenceEditDialog::onDialogClicked);
}

bool SequenceEditDialog::checkUnsavedChanges()
{
    if (isHaveUnsavedChanges()) {
        auto result = QMessageBox::warning(this, tr("Save changes?"),
            tr("Sequence '%1' have unsaved changes.").arg(m_name_edit->text()),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Cancel);

        if (result == QMessageBox::Save) {
            m_mapper->submit();
        }
        else if (result == QMessageBox::Discard){
            m_mapper->revert();
        }
        else {
            return false;
        }
    }

    return true;
}

bool SequenceEditDialog::isHaveUnsavedChanges() const
{
    auto model = m_mapper->model();
    auto row = m_mapper->currentIndex();

    // Check name changes
    auto name = model->data( model->index(row, SequenceModel::kColumnName) ).toString();
    if (name != m_name_edit->text()) {
        return true;
    }

    // Check format changes
    auto desc = model->data( model->index(row, SequenceModel::kColumnDescription) ).toString();
    if (desc != m_desc_editor->toPlainText()) {
        return true;
    }

    // Check text changes
    auto dsl = model->data( model->index(row, SequenceModel::kColumnDsl) ).toString();
    if (dsl != m_dsl_editor->toPlainText()) {
        return true;
    }

    // Check syntax id
    auto syntax =  model->data( model->index(row, SequenceModel::kColumnSyntaxId) ).toString();
    if (syntax != m_format_selection_wgt->selectedFormat()) {
        return true;
    }

    return false;
}

bool SequenceEditDialog::event(QEvent* event)
{
    if (event->type() == QEvent::Close) {
        if (!checkUnsavedChanges()) {
            event->ignore();
            return false;
        }
    }

    return QDialog::event(event);
}


void SequenceEditDialog::onDialogClicked(QAbstractButton* aBtn)
{
    if( m_mapper != nullptr ) {
        auto model = qobject_cast<SequenceModel*>(m_mapper->model());
        if (model == nullptr) {
            return;
        }

        switch( m_dialog_btn->standardButton( aBtn ) )
        {
            case QDialogButtonBox::Apply:
                m_mapper->submit();
                break;

            case QDialogButtonBox::Ok:
                m_mapper->submit();
                accept();
                break;

            case QDialogButtonBox::Reset:
                m_mapper->revert();
                break;

            case QDialogButtonBox::Cancel:
                if (checkUnsavedChanges()) {
                    hide();
                }
                break;

            default:
                break;
        }
    }
}
