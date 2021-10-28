#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>


#include "LogWidget.h"
#include "LogTableModel.h"
#include "LogItemDelegate.h"
#include "event.h"

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();
    connectSignals();
}


LogWidget::~LogWidget()
{

}

void LogWidget::createGui()
{
    // ---------[BUTTONS]---------- //
    mClrBtn = new QPushButton();
        mClrBtn->setIcon(QIcon(":/icons/delete_cross.svg"));
        mClrBtn->setToolTip("Clear log window");

    mModeBtn = new QPushButton();
        mModeBtn->setText(toString(LogTableModel::kDataFormatHex));
        mModeBtn->setFixedSize(64, 32);

    mChangeStateBtn = new QPushButton();
        mChangeStateBtn->setText("Active");
        mChangeStateBtn->setFixedSize(64, 32);

    mConfigBtn = new QPushButton();
        mConfigBtn->setText("115200, none, 8, 1");
        mConfigBtn->setFixedSize(136, 32);

    // ---------[LINE EDIT]---------- //
    mFindLe = new QLineEdit();
        mFindLe->setPlaceholderText(tr("Find sequence"));
        mFindLe->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

    mMessageLe = new QLineEdit();
        mMessageLe->setPlaceholderText("Print your message");


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

    // ---------[TEXT LOG]---------- //
    mLogView = new QTableView();
    mLogModel = new LogTableModel(mLogView);
        mLogModel->setDataFormat(LogTableModel::kDataFormatHex);
        mLogView->setItemDelegate(new LogItemDelegate());
        mLogView->setModel(mLogModel);
        mLogView->setWordWrap(true);
        mLogView->hideColumn(LogTableModel::kColumnUser);
        mLogView->setSelectionBehavior(QAbstractItemView::SelectRows);
        mLogView->setShowGrid(false);
        mLogView->verticalHeader()->hide();
        mLogView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        auto headerView = mLogView->horizontalHeader();
            headerView->setSectionResizeMode(LogTableModel::kColumnTimestamp, QHeaderView::ResizeToContents);
            headerView->setSectionResizeMode(LogTableModel::kColumnChannel,   QHeaderView::ResizeToContents);
            headerView->setSectionResizeMode(LogTableModel::kColumnBytes,     QHeaderView::Stretch);
            headerView->hide();


    // ---------[LAYOUT]---------- //
    auto top_layout = new QHBoxLayout();
        top_layout->addWidget(mClrBtn);
        top_layout->addWidget(mFindLe);
        top_layout->addWidget(mModeBtn);
        top_layout->addWidget(mChangeStateBtn);
        top_layout->addWidget(mConfigBtn);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(mMessageLe);
        bottom_layout->addWidget(edit_mode_cmb);
        bottom_layout->addWidget(editor);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(top_layout, 0, 0);
      layout->addWidget(mLogView, 1, 0);
      layout->addLayout(bottom_layout, 2, 0);

    setLayout(layout);

    // --------[TEST]-------- //
    auto eventFirst = Event{ .channel = 0, .timestamp = QDateTime::currentDateTime(), .bytes = QByteArray(12, '1'), .userData = QStringList()  };

    auto eventSecond = Event{ .channel = 1, .timestamp = QDateTime::currentDateTime(), .bytes = QByteArray(90, '7'), .userData = QStringList()  };

    mLogModel->append(eventFirst);
    mLogModel->append(eventSecond);
}

void LogWidget::connectSignals()
{
    connect(mChangeStateBtn, &QPushButton::released, this, [this]() {
        static bool state = true;
        if (state) {
            mChangeStateBtn->setText("Disabled");
            state = false;
        }
        else
        {
            mChangeStateBtn->setText("Active");
            state = true;
        }
    });

    connect(mClrBtn, &QPushButton::released, this, [this] {
        QMessageBox msgbox;
        {
            msgbox.setText( tr("Clear log?") );
            msgbox.setInformativeText( tr("All sequence will be removed") );
            msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgbox.setDefaultButton(QMessageBox::No);
            msgbox.setIcon(QMessageBox::Icon::Warning);
            msgbox.setFixedSize( QSize(680, 240) );

                msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
                msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
        }

        if (msgbox.exec() == QMessageBox::Yes) {
            mLogModel->clear();
        }
    });

    connect(mModeBtn, &QPushButton::released, this, [this]() {
       auto format = mLogModel->dataFormat();
       format = (format == LogTableModel::kDataFormatHex) ?
                   LogTableModel::kDataFormatAscii : LogTableModel::kDataFormatHex;

       mLogModel->setDataFormat(format);
       mModeBtn->setText(toString(format));
    });
}


void LogWidget::showDialog() {
}

