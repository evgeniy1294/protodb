#pragma once

#include <QMenu>

class QMouseEvent;

namespace protodb {

class isa_menu : public QMenu
{
  Q_OBJECT

public:
  explicit isa_menu(QWidget *parent = nullptr);
  virtual ~isa_menu() override = default;

  void mouseReleaseEvent( QMouseEvent * event ) override;
};

} // namespace protodb
