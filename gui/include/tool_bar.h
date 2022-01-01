#pragma once

#include <QList>
#include <QtWidgets/QWidget>
#include <QBoxLayout>
#include <QToolButton>
#include <QSize>

class QToolButton;
class QMenu;
class QAction;
class isa_menu;

class isa_tool_bar : public QWidget
{
    Q_OBJECT
public:
    explicit isa_tool_bar( QBoxLayout::Direction direction, QWidget *parent = nullptr );
    virtual ~isa_tool_bar() override = default;

    // Работа с действиями
    void addToolAction( QAction * action,
                        bool quick_tool_bar = true,
                        QToolButton::ToolButtonPopupMode = QToolButton::DelayedPopup );
    void prependToolAction( QAction * action,
                        bool quick_tool_bar = true,
                        QToolButton::ToolButtonPopupMode = QToolButton::DelayedPopup );
    void removeToolAction( QAction * action );
    QList< QAction* > getActions( ) const;

    void addMenuSeparator();

    // Размер кнопок
    void setButtonSize( int getButtonSize );
    void setButtonSize( QSize getButtonSize );
    QSize getButtonSize() const;

    // Расстояние между кнопками
    void setSpacing(int spacing);
    int getSpacing() const;

    // Отступы
    void setContentsMargins(int left, int top, int right, int bottom);
    int getLeftMergin() const;
    int getTopMergin() const;
    int getRightMergin() const;
    int getBottomMergin() const;

    // Положение кнопки со вызывающей меню со списком всех действий
    void setAllActionMenuFirst(bool v);
    bool getAllActionMenuFirst() const;

    // Отображать или нет кнопку вызова меню с подробным списком действий
    void setAllActionButtonVisible(bool b);
    bool getAllActionButtonVisible() const;

    // Отображать или нет действия "быстрого доступа"
    void setQuickToolBarVisible(bool b);
    bool getQuickToolBarVisible() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;

private slots:
    void on_action_changed();
    void on_action_destroyed(QObject *obj);

private:
    void create_gui();

    void try_visible();
    void calc_sizes();
    void calc_buttons_positions();


    bool m_all_action_menu_first = true;
    QSize m_size = QSize( 24, 24 );

    int m_spacing = 3;

    int m_left_mergin   = 0;
    int m_top_mergin    = 0;
    int m_right_mergin  = 0;
    int m_bottom_mergin = 0;

    bool m_all_actions_menu_visible = true;
    bool m_quick_toolbar_visible = true;

    struct Buttons
    {
        QToolButton* button;
        QAction* action;
    };

    QBoxLayout::Direction m_direction;

    QList<Buttons> m_buttons;
    QToolButton * m_show_full_buttons;

    isa_menu *m_buttons_menu;

    Q_DISABLE_COPY(isa_tool_bar)
};
