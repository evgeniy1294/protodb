#include <QWidget>
#include <QLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>

#include "script_widget.h"

ScriptWidget::ScriptWidget(QWidget* parent)
  : QWidget(parent)
  , m_file(new QFile())
{
    createGui();
    connectSignals();
}

void ScriptWidget::createGui()
{
    // ---------[EDITOR]---------- //
    m_text = new QTextEdit();
        m_text->setText(tr("Scripting"));

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

    // ---------[LAYOUT]---------- //
    auto tool_layout = new QHBoxLayout();
        tool_layout->addWidget(m_new_btn);
        tool_layout->addWidget(m_open_btn);
        tool_layout->addWidget(m_save_btn);
        tool_layout->addWidget(m_file_le);


    auto main_layout = new QGridLayout();
        main_layout->addLayout(tool_layout, 0, 0);
        main_layout->addWidget(m_text, 1, 0);

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

            if (m_file->isOpen()) {
                m_file->close();
            }

            m_file_le->setText(path);
            m_file->open(QIODevice::ReadWrite);
            m_file->resize(0);
            m_text->setText(m_file->readAll());
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

            if (m_file->isOpen()) {
                m_file->close();
            }

            m_file_le->setText(path);
            m_file->setFileName(path);
            m_file->open(QIODevice::ReadWrite);
            m_text->clear();
            m_text->setText(m_file->readAll());
      }
    }
}

void ScriptWidget::save()
{
    if (m_file->isOpen() && m_file->isWritable()) {
        if (m_file->resize(0)) {
            m_file->write(m_text->toPlainText().toLatin1());
        }
    }
}

