#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
//#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QDialogButtonBox>

#include "sq_table_dialog.h"

SqTableDialog::SqTableDialog(QWidget* aParent)
    : QDialog(aParent)
    , mSelectedRow(-1)
    , mMapper(nullptr)
{
    createGui();
    createConnections();
}

QDataWidgetMapper* SqTableDialog::getMapper() const
{
    return mMapper;
}

void SqTableDialog::setMapper(QDataWidgetMapper* aMapper)
{
    mMapper = aMapper;
}

void SqTableDialog::selectRow(int aRow)
{
    mSelectedRow = aRow;
}

int SqTableDialog::getRow() const
{
    return mSelectedRow;
}

void SqTableDialog::createGui()
{
    setWindowTitle(tr("Sequence Editor"));

    auto h_layout = new QHBoxLayout();

        auto btn_back  = new QPushButton();
            btn_back->setIcon(QIcon(":/icons/arrow_back.svg"));
            connect(btn_back, &QPushButton::released, this, &SqTableDialog::onBackBtnClicked);

        auto btn_prev  = new QPushButton();
            btn_prev->setIcon(QIcon(":/icons/arrow_left.svg"));
            connect(btn_prev, &QPushButton::released, this, &SqTableDialog::onPrevBtnClicked);

        auto btn_next  = new QPushButton();
            btn_next->setIcon(QIcon(":/icons/arrow_right.svg"));
            connect(btn_next, &QPushButton::released, this, &SqTableDialog::onNextBtnClicked);

        auto btn_front = new QPushButton();
            btn_front->setIcon(QIcon(":/icons/arrow_front.svg"));
            connect(btn_front, &QPushButton::released, this, &SqTableDialog::onFrontBtnClicked);

        auto btn_add   = new QPushButton();
            btn_add->setIcon(QIcon(":/icons/add.svg"));
            connect(btn_add, &QPushButton::released, this, &SqTableDialog::onAddBtnClicked);

        //auto btn_rm   = new QPushButton();
            //btn_rm->setIcon(QIcon(":/icons/close.svg"));

        itemLabel = new QLabel(tr("0/0"));

        h_layout->addWidget(btn_back);
        h_layout->addWidget(btn_prev);
        h_layout->addWidget(itemLabel);
        h_layout->addWidget(btn_next);
        h_layout->addWidget(btn_front);
        h_layout->addStretch();
        h_layout->addWidget(btn_add);
        //h_layout->addWidget(btn_rm);

    auto mNameLe = new QLineEdit();
        mNameLe->setPlaceholderText(tr("Sequence name"));

    auto mDescrEditor = new QTextBrowser();
        mDescrEditor->setPlaceholderText(tr("Description..."));

    auto mSeqEditor = new QTextBrowser();
        mSeqEditor->setPlaceholderText(tr("CRC:Modbus{bytes}"));

    mButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok|
                                       QDialogButtonBox::Apply|
                                       QDialogButtonBox::Reset|
                                       QDialogButtonBox::Cancel );
        connect( mButtonBox, &QDialogButtonBox::clicked, this, &SqTableDialog::onDialogBtnClicked );

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(mNameLe,      1, 0);
        main_layout->addWidget(mDescrEditor, 2, 0);
        main_layout->addWidget(mSeqEditor,   3, 0);
        main_layout->addWidget(mButtonBox,   4, 0);

    setLayout(main_layout);
}

void SqTableDialog::createConnections()
{

}

void SqTableDialog::onDialogBtnClicked(QAbstractButton* aBtn)
{
    if( mMapper != nullptr ) {

        switch( mButtonBox->standardButton( aBtn ) )
        {
            case QDialogButtonBox::Apply:
                break;

            case QDialogButtonBox::Ok:
                break;

            case QDialogButtonBox::Reset:
                break;

            case QDialogButtonBox::Cancel:
                break;

            default:
                break;
        }
    }
}

void SqTableDialog::onBackBtnClicked()
{

}

void SqTableDialog::onPrevBtnClicked()
{

}

void SqTableDialog::onNextBtnClicked()
{

}

void SqTableDialog::onFrontBtnClicked()
{

}

void SqTableDialog::onAddBtnClicked()
{

}
