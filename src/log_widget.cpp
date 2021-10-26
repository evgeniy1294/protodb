#include <QWidget>
#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>


#include "log_widget.h"
#include "log_model.h"
#include "event.h"

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

    // ---------[FIND LINE EDIT]---------- //
    mFindLe = new QLineEdit();
        mFindLe->setPlaceholderText(tr("Find sequence"));
        mFindLe->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

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

    mLogView = new QTableView();
    auto mLogModel = new LogModel(mLogView);
        mLogView->setModel(mLogModel);
        mLogView->hideColumn(LogModel::kColumnUser);
        mLogView->setSelectionBehavior(QAbstractItemView::SelectRows);
        mLogView->setShowGrid(false);
        mLogView->verticalHeader()->hide();
        mLogView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        auto headerView = mLogView->horizontalHeader();
            headerView->setSectionResizeMode(LogModel::kColumnTimestamp, QHeaderView::ResizeToContents);
            headerView->setSectionResizeMode(LogModel::kColumnChannel,   QHeaderView::ResizeToContents);
            headerView->setSectionResizeMode(LogModel::kColumnBytes,     QHeaderView::Stretch);
            headerView->hide();


    // ---------[LAYOUT]---------- //
    auto tool_layout = new QHBoxLayout();
        tool_layout->addWidget(clr_btn);
        tool_layout->addWidget(mFindLe);
        tool_layout->addWidget(mModeLabel);
        tool_layout->addWidget(mStatusLabel);
        tool_layout->addWidget(mConfigLabel);

    auto edit_layout = new QHBoxLayout();
        edit_layout->addWidget(msg_edit);
        edit_layout->addWidget(edit_mode_cmb);
        edit_layout->addWidget(editor);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(tool_layout, 0, 0);
      layout->addWidget(mLogView, 1, 0);
      layout->addLayout(edit_layout, 2, 0);

    setLayout(layout);

    // --------[TEST]-------- //
    auto eventFirst = Event{ .channel = 0, .timestamp = QDateTime::currentDateTime(), .bytes = QByteArray(12, '1'), .userData = QStringList()  };

    auto eventSecond = Event{ .channel = 1, .timestamp = QDateTime::currentDateTime(), .bytes = QByteArray(90, '7'), .userData = QStringList()  };

    mLogModel->append(eventFirst);
    mLogModel->append(eventSecond);
}


void LogWidget::showDialog() {
}

