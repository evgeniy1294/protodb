#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QLayout>
#include <QTextBrowser>
#include <QFontComboBox>
#include <QStringList>


#include "LogConfigWidget.h"

static const QStringList font_size_list = { "6",  "7",  "8",  "9", "10", "11", "12", "14", "16",
                                           "18", "20", "22", "24", "26", "28", "36", "48", "72"};

LogConfigWidget::LogConfigWidget(QWidget* aParent)
    : QFrame(aParent)
{
    createGui();
    connectSignals();
}




void LogConfigWidget::createGui() {

    auto channel_label_1       = new QLabel(tr("Channel 1 color"));
    auto channel_label_2       = new QLabel(tr("Channel 2 color"));
    auto channel_label_service = new QLabel(tr("Service channel color"));
    auto channel_label_user    = new QLabel(tr("User channel color"));
    auto background_label      = new QLabel(tr("Background color"));
    auto font_label            = new QLabel(tr("Font"));

    auto pixmap = QPixmap(QSize(36,36));

    mCh1ColorBtn = new QPushButton();
        pixmap.fill(Qt::red);
        mCh1ColorBtn->setIcon(QIcon(pixmap));
        mCh1ColorBtn->setFixedSize(48, 32);

    mCh2ColorBtn = new QPushButton();
        pixmap.fill(Qt::blue);
        mCh2ColorBtn->setIcon(QIcon(pixmap));
        mCh2ColorBtn->setFixedSize(48, 32);

    mChSrvColorBtn = new QPushButton();
        pixmap.fill(Qt::green);
        mChSrvColorBtn->setIcon(QIcon(pixmap));
        mChSrvColorBtn->setFixedSize(48, 32);

    mChUsrColorBtn = new QPushButton();
         pixmap.fill(Qt::magenta);
         mChUsrColorBtn->setIcon(QIcon(pixmap));
         mChUsrColorBtn->setFixedSize(48, 32);


    mBkgColorBtn = new QPushButton();
         pixmap.fill(Qt::white);
         mBkgColorBtn->setIcon(QIcon(pixmap));
         mBkgColorBtn->setFixedSize(48, 32);

    mFontCb = new QFontComboBox();

    mFontSizeCb = new QComboBox();
        mFontSizeCb->addItems(font_size_list);

    auto layout = new QGridLayout();
        layout->addWidget(channel_label_1, 0, 0);
        layout->addWidget(channel_label_2, 1, 0);
        layout->addWidget(channel_label_service, 2, 0);
        layout->addWidget(channel_label_user, 3, 0);
        layout->addWidget(background_label, 4, 0);
        layout->addWidget(font_label, 5, 0);
        layout->addWidget(mCh1ColorBtn, 0, 1);
        layout->addWidget(mCh2ColorBtn, 1, 1);
        layout->addWidget(mChSrvColorBtn, 2, 1);
        layout->addWidget(mChUsrColorBtn, 3, 1);
        layout->addWidget(mBkgColorBtn, 4, 1);
        layout->addWidget(mFontSizeCb, 5, 1);
        layout->addWidget(mFontCb, 5, 2);
        layout->addWidget(new QTextBrowser, 6, 0, 1, 3);

        layout->setColumnStretch(0, 0);
        layout->setColumnStretch(1, 0);
        layout->setColumnStretch(2, 1);

    setLayout(layout);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
}

void LogConfigWidget::connectSignals()
{
    connect(mCh1ColorBtn, &QPushButton::released, this, &LogConfigWidget::colorDialog);
    connect(mCh2ColorBtn, &QPushButton::released, this, &LogConfigWidget::colorDialog);
    connect(mChSrvColorBtn, &QPushButton::released, this, &LogConfigWidget::colorDialog);
    connect(mChUsrColorBtn, &QPushButton::released, this, &LogConfigWidget::colorDialog);
    connect(mBkgColorBtn, &QPushButton::released, this, &LogConfigWidget::colorDialog);
}


void LogConfigWidget::colorDialog() {
    QColorDialog dialog;
    {
        dialog.setOption(QColorDialog::ShowAlphaChannel, false);
    }

    if (dialog.exec() == 1) {
        auto pixmap = QPixmap(QSize(36,36));
            pixmap.fill(dialog.selectedColor());

        auto signalSender = sender();

        if (signalSender == mCh1ColorBtn) {
            mCh1ColorBtn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == mCh2ColorBtn) {
            mCh2ColorBtn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == mChSrvColorBtn) {
            mChSrvColorBtn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == mChUsrColorBtn) {
            mChUsrColorBtn->setIcon(QIcon(pixmap));
        }
        else if (signalSender == mBkgColorBtn) {
            mBkgColorBtn->setIcon(QIcon(pixmap));
        }
    }
}
