#pragma once

#include <QDialog>
#include <QPointer>

class QDialogButtonBox;
class QDataWidgetMapper;
class QAbstractButton;
class QLabel;
class QLineEdit;
class QTextBrowser;
class QTextEdit;

class SqTableDialog: public QDialog
{
    Q_OBJECT

public:
    SqTableDialog( QWidget* aParent = nullptr );
    ~SqTableDialog() = default;

    QDataWidgetMapper* mapper() const;
    void setMapper(QDataWidgetMapper* aMapper);

    void wipe();

private slots:
    void onDialogBtnClicked(QAbstractButton* aBtn);

private:
    void createGui();
    void connectSignals();

private:
    QPushButton* btn_back;
    QPushButton* btn_prev;
    QPushButton* btn_next;
    QPushButton* btn_front;
    QPushButton* btn_add;
    QLabel* itemLabel;
    QLineEdit* mNameLe;
    QTextEdit* mDescrEditor;
    QTextEdit* mSeqEditor;
    QDataWidgetMapper* mMapper;
    QDialogButtonBox* mButtonBox;
};
