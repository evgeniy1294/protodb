#include <QWidget>
#include <QLayout>
#include <QTextBrowser>

#include "workarea_widget.h"
#include "log_widget.h"
#include "editor_widget.h"
#include "script_widget.h"

WorkAreaWidget::WorkAreaWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


WorkAreaWidget::~WorkAreaWidget()
{

}


void WorkAreaWidget::createGui()
{
  QGridLayout* layout = new QGridLayout(this);    
    layout->addWidget(new LogWidget(), 0, 0, 1, 1);
    layout->addWidget(new EditorWidget(), 1, 0, 1, 1);
    layout->addWidget(new ScriptWidget(), 0, 1, 2, 1);
}
