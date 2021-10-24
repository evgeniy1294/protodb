#include <QWidget>
#include <QLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPixmap>
#include <QColorDialog>
#include <QLabel>
#include <QTextCursor>

#include "log_widget.h"

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();
}


LogWidget::~LogWidget()
{

}

void LogWidget::createGui()
{
    // ---------[BUTTONS]---------- //
    auto clr_btn = new QPushButton();
        clr_btn->setIcon(QIcon(":/icons/delete_cross.svg"));
        clr_btn->setToolTip("Clear log window");
        connect(clr_btn, &QPushButton::released, this, &LogWidget::showDialog);

    // ---------[LABEL]---------- //
    mModeLabel = new QLabel("HEX");
        mModeLabel->setFrameShape(QFrame::StyledPanel);
        mModeLabel->setFrameShadow(QFrame::Raised);
        mModeLabel->setLineWidth(3);
        mModeLabel->setFixedSize(64, 32);
        mModeLabel->setAlignment(Qt::AlignCenter);

    mStatusLabel = new QLabel("Active");
        mStatusLabel->setFrameShape(QFrame::StyledPanel);
        mStatusLabel->setFrameShadow(QFrame::Raised);
        mStatusLabel->setLineWidth(3);
        mStatusLabel->setFixedSize(64, 32);
        mStatusLabel->setAlignment(Qt::AlignCenter);

    mConfigLabel = new QLabel("115200, none, 8, 1");
        mConfigLabel->setFrameShape(QFrame::StyledPanel);
        mConfigLabel->setFrameShadow(QFrame::Raised);
        mConfigLabel->setLineWidth(3);
        mConfigLabel->setFixedSize(136, 32);
        mConfigLabel->setAlignment(Qt::AlignCenter);



    // ---------[LINE EDIT]---------- //
    auto msg_edit = new QLineEdit();
        msg_edit->setPlaceholderText("Your message");


    // ---------[COMBO BOX]---------- //
    auto tool_mode_cmb = new QComboBox();
        tool_mode_cmb->addItem("HEX");
        tool_mode_cmb->addItem("ASCII");

    auto edit_mode_cmb = new QComboBox();
        edit_mode_cmb->addItem("HEX");
        edit_mode_cmb->addItem("LF");
        edit_mode_cmb->addItem("CR");
        edit_mode_cmb->addItem("CR/LF");
        edit_mode_cmb->addItem("None");
        edit_mode_cmb->addItem("DSL");

    // ---------[SPIN BOX]---------- //
    auto editor = new QSpinBox();
        editor->setMinimum(0);
        editor->setMaximum(120);
        editor->setSingleStep(1);
        editor->setSuffix("ms");
        editor->setSpecialValueText(QObject::tr("No delay"));

    // ---------[TEXT BROWSER]---------- //
    QTextCharFormat channelOneFormat = QTextCharFormat();
        channelOneFormat.setForeground(Qt::red);

    QTextCharFormat channelTwoFormat = QTextCharFormat();
        channelTwoFormat.setForeground(Qt::blue);

    QTextCharFormat channelSrvFormat = QTextCharFormat();
        channelSrvFormat.setForeground(Qt::darkGreen);


    QTextBlockFormat blockFormat = QTextBlockFormat();
        blockFormat.setForeground(Qt::blue);

    mLogBrowser = new QTextBrowser();
        auto cursor(mLogBrowser->textCursor());
            cursor.beginEditBlock();
                    cursor.setCharFormat(channelSrvFormat);
                    cursor.insertText(tr("13:43:31 <<< "));
                    cursor.setCharFormat(channelOneFormat);
                    cursor.insertText(tr("received sequence"));
                cursor.insertBlock();
                    cursor.setCharFormat(channelSrvFormat);
                    cursor.insertText(tr("13:45:32 >>> "));
                    cursor.setCharFormat(channelTwoFormat);
                    cursor.insertText(tr("sended sequence"));

            cursor.endEditBlock();

    // ---------[LAYOUT]---------- //
    auto tool_layout = new QHBoxLayout();
        tool_layout->addWidget(clr_btn);
        tool_layout->addStretch();
        tool_layout->addWidget(mModeLabel);
        tool_layout->addWidget(mStatusLabel);
        tool_layout->addWidget(mConfigLabel);

    auto edit_layout = new QHBoxLayout();
        edit_layout->addWidget(msg_edit);
        edit_layout->addWidget(edit_mode_cmb);
        edit_layout->addWidget(editor);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(tool_layout, 0, 0);
      layout->addWidget(mLogBrowser, 1, 0);
      layout->addLayout(edit_layout, 2, 0);

    setLayout(layout);
}


void LogWidget::showDialog() {
}

