#include <QWidget>
#include <QLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QTextDocument>
#include <QTextStream>
#include <QMessageBox>
#include <CodeEditorWidget.h>

#include "script_widget.h"

ScriptWidget::ScriptWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();
    connectSignals();
}

void ScriptWidget::createGui()
{
    // ---------[EDITOR]---------- //
    m_editor = new QPlainTextEdit();

    // ---------[BUTTONS]--------- //
    m_new_btn = new QPushButton();
        m_new_btn->setIcon(QIcon(":/icons/file.svg"));
        m_new_btn->setToolTip(tr("New file"));
        m_new_btn->setFixedSize(32, 32);

    m_open_btn = new QPushButton();
        m_open_btn->setIcon(QIcon(":/icons/folder.svg"));
        m_open_btn->setToolTip(tr("Open file"));
        m_open_btn->setFixedSize(32, 32);

    m_save_btn = new QPushButton();
        m_save_btn->setIcon(QIcon(":/icons/floppy.svg"));
        m_save_btn->setToolTip(tr("Save file"));
        m_save_btn->setFixedSize(32, 32);

    m_file_le = new QLineEdit();
        m_file_le->setText("/tmp/protodb/script.lua");
        m_file_le->setPlaceholderText("Path to script file");
        m_file_le->setReadOnly(true);

    m_code = new CodeEditor;
    // ---------[LAYOUT]---------- //
    auto tool_layout = new QHBoxLayout();
        tool_layout->addWidget(m_new_btn);
        tool_layout->addWidget(m_open_btn);
        tool_layout->addWidget(m_save_btn);
        tool_layout->addWidget(m_file_le);


    auto main_layout = new QGridLayout();
        main_layout->addLayout(tool_layout, 0, 0);
        main_layout->addWidget(m_code, 1, 0);

        setLayout(main_layout);
}

void ScriptWidget::connectSignals()
{
    connect(m_new_btn,  &QPushButton::released, this, &ScriptWidget::create);
    connect(m_open_btn, &QPushButton::released, this, &ScriptWidget::open);
    connect(m_save_btn, &QPushButton::released, this, &ScriptWidget::save);
}

void ScriptWidget::create()
{
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::List);

    QStringList fileNames;
    if (fileDialog.exec()) {
        fileNames = fileDialog.selectedFiles();

        if (fileNames.size() != 0) {
            auto path = fileNames.back();
            m_file_le->setText(path);

            QFile file(path);
            if (file.open(QIODevice::Text | QIODevice::Truncate)) {
                m_code->setPlainText("");
            }
            else {
                showFileError(file, "Error while create file");
            }

            file.close();
        }
    }
}

void ScriptWidget::open()
{
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setViewMode(QFileDialog::List);

    QStringList fileNames;
    if (fileDialog.exec()) {
        fileNames = fileDialog.selectedFiles();

        if (fileNames.size() != 0) {
            auto path = fileNames.back();
            m_file_le->setText(path);

            QFile file(path);
            if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
                QString content;

                while(!file.atEnd()) {
                    content.append(file.readLine());
                }

                m_code->setPlainText(content);

                auto higlighter = new Highlighter(m_code->document());

            }
            else {
                showFileError(file, "Error while open file");
            }

            file.close();
        }
    }
}

void ScriptWidget::save()
{
    QFile file(m_file_le->text());

    if (file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);
        out << m_code->toPlainText();
    }
    else {
        showFileError(file, "Error while saving file");
    }

    file.close();
}

void ScriptWidget::showFileError(const QFile& file, const QString& text)
{
    QMessageBox box;
        box.setIcon(QMessageBox::Critical);

    QString message = tr("%1: %2: %3").
            arg(text, file.fileName(), file.errorString());

    box.setText(message);
    box.exec();
}
