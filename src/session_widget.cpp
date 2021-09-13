#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include "session_widget.h"


SessionWidget::SessionWidget(QWidget *parent)
  : QWidget(parent)
{
  createGui();

}



SessionWidget::~SessionWidget()
{

}


void SessionWidget::createGui()
{
  auto mLayout = new QHBoxLayout(this);

    mSplitter = new QSplitter();
    mSplitter->setOrientation(Qt::Horizontal);
    mSplitter->setCollapsible(0, true);
//    mSplitter->setHandleWidth(5);
//    mSplitter->setStyleSheet("QSplitter::handle {background: red;}");
//    mSplitter->setChildrenCollapsible(true);

//      mLabel = new QLabel(tr("Im label!"));
      auto mSplitterLabel1 = new QLabel(tr("Im splitter!"));
      mSplitterLabel1->setStyleSheet("QLabel {background: red;}");
      auto mSplitterLabel2 = new QLabel(tr("Im splitter!"));
      mSplitterLabel2->setStyleSheet("QLabel {background: red;}");

//    mSplitter->addWidget(mLabel);
    mSplitter->addWidget(mSplitterLabel1);
    mSplitter->addWidget(mSplitterLabel2);

  mLayout->addWidget(mSplitter);
}
