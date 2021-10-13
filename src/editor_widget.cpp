#include <QAction>
#include <QLayout>
#include <QTextBrowser>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QDebug>
#include "editor_widget.h"
#include "singleton.h"


EditorWidget::EditorWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();
}

void EditorWidget::onSaveClicked()
{
  Sequence sq;
  sq.setName(mNameLe->text());
  sq.setDescription(mHelpEditor->toPlainText());
  sq.setCharString(mDataEditor->toPlainText());

  Singleton::instance().mSequenceStorage.append(sq);

  qDebug() << "EditorWidget: " << "onToolButtonClicked";
}

void EditorWidget::onResetClicked()
{
  mNameLe->clear();
  mHelpEditor->clear();
  mDataEditor->clear();
}

void EditorWidget::createActions()
{
  mSaveAct  = new QAction(QIcon(":/icons/save.svg"), tr("&Save"), this);
    connect(mSaveAct, &QAction::triggered, this, &EditorWidget::onSaveClicked);

  mResetAct = new QAction(QIcon(":/icons/reset.svg") , tr("&Reset"), this);
    connect(mResetAct, &QAction::triggered, this, &EditorWidget::onResetClicked);
}

void EditorWidget::createMenu()
{
  mToolMenu = new QMenu();
    mToolMenu->addAction(mSaveAct);
    mToolMenu->addSeparator();
    mToolMenu->addAction(mResetAct);
}

void EditorWidget::createGui()
{
  createActions();
  createMenu();

  mNameLe = new QLineEdit();
    mNameLe->setPlaceholderText(tr("Sequence name"));

  mStatusLabel = new QLabel(tr("HEX, pos 4/7"));

  mHelpEditor = new QTextBrowser();
    mHelpEditor->setPlaceholderText(tr("Description..."));

  mDataEditor = new QTextBrowser();
    mDataEditor->setPlaceholderText(tr("CRC:Modbus{bytes}"));

  mToolButton = new QToolButton();
    mToolButton->setIcon(QIcon(":/icons/save.svg"));
    mToolButton->setMenu(mToolMenu);
    connect(mToolButton, &QToolButton::clicked, this, &EditorWidget::onSaveClicked);

  QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(mNameLe, 0, 0);
    layout->addWidget(mToolButton, 0, 1);

    layout->addWidget(mHelpEditor, 1, 0);
    layout->addWidget(mStatusLabel, 2, 0);
    layout->addWidget(mDataEditor, 3, 0);
}

