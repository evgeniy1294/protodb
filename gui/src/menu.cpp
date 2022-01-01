#include <QMouseEvent>
#include <protodb/menu.h>

isa_menu::isa_menu(QWidget *parent)
    : QMenu(parent)
{
    setToolTipsVisible( true );
}

void isa_menu::mouseReleaseEvent( QMouseEvent *event )
{
  QAction* act = menuAction();
  if( act )
  {
    QMenu* men = act->menu();
      act->setMenu( nullptr );

    QMenu::mouseReleaseEvent(event);
      act->setMenu(men);
  }
  else
  {
    QMenu::mouseReleaseEvent(event);
  }
}
