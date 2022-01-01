#pragma once

#include <QWidget>

class QPlainTextEdit;
class QTextDocument;
class QPushButton;
class QLineEdit;
class QFile;
class CodeEditor;

class ScriptWidget: public QWidget
{
  Q_OBJECT

public:
    explicit ScriptWidget(QWidget* parent = nullptr);
    ~ScriptWidget() = default;

private:
    void createGui();
    void connectSignals();
    void showFileError(const QFile& file, const QString& text);

private slots:
    void create();
    void open();
    void save();

private:
    QPlainTextEdit* m_editor;
    QPushButton* m_new_btn;
    QPushButton* m_open_btn;
    QPushButton* m_save_btn;
    QLineEdit* m_file_le;
    QTextDocument* m_doc;

    CodeEditor* m_code;
};


