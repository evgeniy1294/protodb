#pragma once

#include <QWidget>

class QTextEdit;
class QPushButton;
class QLineEdit;
class QFile;

class ScriptWidget: public QWidget
{
  Q_OBJECT

public:
    explicit ScriptWidget(QWidget* parent = nullptr);
    ~ScriptWidget() = default;

private:
    void createGui();
    void connectSignals();

private slots:
    void create();
    void open();
    void save();

private:
    QTextEdit* m_text;
    QPushButton* m_new_btn;
    QPushButton* m_open_btn;
    QPushButton* m_save_btn;
    QLineEdit* m_file_le;
    QFile* m_file;
};


