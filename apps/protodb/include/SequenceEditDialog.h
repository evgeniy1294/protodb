#pragma once

#include <QDialog>
#include <QPointer>

class QDialogButtonBox;
class QDataWidgetMapper;
class QAbstractButton;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QGroupBox;
class QRadioButton;

class SequenceEditDialog: public QDialog
{
    Q_OBJECT

public:
    SequenceEditDialog( QWidget* aParent = nullptr );
    ~SequenceEditDialog() = default;

    QDataWidgetMapper* mapper() const;
    void setMapper(QDataWidgetMapper* aMapper);

    void reload();

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    void createGui();
    void createConnections();
    void createSyntaxSelector();
    void setFormatSelection(const QString& format_id);
    QString getSelectedFormat() const;

private:
    QPushButton* m_back_btn;
    QPushButton* m_prev_btn;
    QPushButton* m_next_btn;
    QPushButton* m_front_btn;
    QPushButton* m_add_btn;
    QLabel* m_item_label;
    QLineEdit* m_name_edit;
    QPlainTextEdit* m_desc_editor;
    QPlainTextEdit* m_dsl_editor;
    QGroupBox* m_syntax_selection_group;
    QDataWidgetMapper* m_mapper;
    QDialogButtonBox* m_dialog_btn;
    QList<QRadioButton*> m_syntax_btns;
};
