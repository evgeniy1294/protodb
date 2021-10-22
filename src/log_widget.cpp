#include <QWidget>
#include <QLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPixmap>
#include <QColorDialog>

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

    auto rx_btn_pixmap = QPixmap(QSize(36,36));
        rx_btn_pixmap.fill(Qt::red);
    mRxBtn = new QPushButton(rx_btn_pixmap, tr("RX"));
        mRxBtn->setFlat(true);
        mRxBtn->setFixedSize(48, 36);
        connect(mRxBtn, &QPushButton::released, this, &LogWidget::colorDialog);

    auto tx_pixmap = QPixmap(QSize(36,36));
        tx_pixmap.fill(Qt::green);
    mTxBtn = new QPushButton(tx_pixmap, tr("TX"));
        mTxBtn->setFlat(true);
        mTxBtn->setFixedSize(48, 36);
        connect(mTxBtn, &QPushButton::released, this, &LogWidget::colorDialog);

    // ---------[LINE EDIT]---------- //
    auto msg_edit = new QLineEdit();
        msg_edit->setPlaceholderText("Your message");


    // ---------[COMBO BOX]---------- //
    auto tool_mode_cmb = new QComboBox();
        tool_mode_cmb->setFrame(false);
        tool_mode_cmb->setFixedSize(64, 36);
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
    mLogBrowser = new QTextBrowser();
      mLogBrowser->setText(tr("05.10.21 15:29:53 <<< Connection log"));

    // ---------[LAYOUT]---------- //
    auto tool_layout = new QHBoxLayout();
        tool_layout->addWidget(clr_btn);
        tool_layout->addStretch();
        tool_layout->addWidget(tool_mode_cmb);
        tool_layout->addWidget(mRxBtn);
        tool_layout->addWidget(mTxBtn);

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



void LogWidget::colorDialog() {
    auto s = sender();

    QColorDialog dialog;
    {
        dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    }

    if (dialog.exec() == 1) {
        auto pixmap = QPixmap(QSize(36,36));
            pixmap.fill(dialog.selectedColor());

        if (s == mRxBtn) {
            mRxBtn->setIcon(QIcon(pixmap));
        }
        else {
            mTxBtn->setIcon(QIcon(pixmap));
        }
    }
}

