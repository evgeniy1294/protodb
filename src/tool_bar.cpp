#include <QToolButton>
#include <QMenu>

#include "tool_bar.h"
#include "menu.h"



///
/// \brief isa_tool_bar::isa_tool_bar
/// \param direction
/// \param parent
///
isa_tool_bar::isa_tool_bar(QBoxLayout::Direction direction, QWidget *parent)
    : QWidget(parent)
    , m_direction(direction)
{
    calc_sizes();
    create_gui();
    try_visible();
}


void isa_tool_bar::create_gui()
{
    m_show_full_buttons = new QToolButton(this);
        m_show_full_buttons->setText("...");
        m_show_full_buttons->setToolTip(tr("Все действия"));
    m_show_full_buttons->setIcon(QIcon(":/icons/list.svg"));

    m_buttons_menu = new isa_menu(this);
    connect(m_show_full_buttons, &QToolButton::clicked,
            [this]()
            {
                m_buttons_menu->exec(QCursor::pos());
            }
    );
    //    m_lay_central->addWidget(m_show_full_buttons);
}


///
/// \brief isa_tool_bar::add_tool_action
/// Добавление действий на панель
/// \param action - действие
/// \param quick_tool_bar - флаг добавления действия на панель быстрого доступа (иначе будет только в меню)
///
void isa_tool_bar::addToolAction(QAction *action, bool quick_tool_bar, QToolButton::ToolButtonPopupMode mode )
{
    if(quick_tool_bar)
    {
        QToolButton *btn = new QToolButton(this);
            btn->setDefaultAction( action );
            btn->setPopupMode( mode );

        Buttons b;
            b.action = action;
            b.button = btn;
        m_buttons.append(b);
    }

    m_buttons_menu->addAction(action);
    calc_buttons_positions();
    try_visible();
    updateGeometry();

    connect(action, &QAction::destroyed,
            this, &isa_tool_bar::on_action_destroyed);
    connect(action, &QAction::changed,
            this, &isa_tool_bar::on_action_changed);
}

void isa_tool_bar::prependToolAction(QAction *action, bool quick_tool_bar, QToolButton::ToolButtonPopupMode mode )
{
    if( quick_tool_bar )
    {
        QToolButton *btn = new QToolButton(this);
            btn->setDefaultAction( action );
            btn->setPopupMode( mode );

        Buttons b;
            b.action = action;
            b.button = btn;
        m_buttons.prepend(b);
    }

    const auto &acts = m_buttons_menu->actions();
    if( acts.isEmpty() )
        m_buttons_menu->addAction( action );
    else
        m_buttons_menu->insertAction( acts.at(0), action );

    calc_buttons_positions();
    try_visible();
    updateGeometry();

    connect(action, &QAction::destroyed,
             this, &isa_tool_bar::on_action_destroyed);
    connect(action, &QAction::changed,
             this, &isa_tool_bar::on_action_changed);
}

///
/// \brief isa_tool_bar::removeToolAction
/// Удалление действия с панели
/// \param action
///
void isa_tool_bar::removeToolAction(QAction *action)
{
    for(int i = 0; i < m_buttons.size(); ++i)
    {
        if(m_buttons.at(i).action == action)
        {
            delete m_buttons.at(i).button;
            m_buttons.removeAt(i);
            disconnect( action, 0, this, 0 );
            break;
        }
    }

    m_buttons_menu->removeAction(action);
    calc_buttons_positions();
    try_visible();
    updateGeometry();
}

///
/// \brief isa_tool_bar::getActions
/// Возвращает список всех действий
/// \return
///
QList<QAction*> isa_tool_bar::getActions() const
{
    return m_buttons_menu->actions();
}

///
/// \brief isa_tool_bar::setSpacing
/// Расстояние между кнопками
/// \param spacing
///
void isa_tool_bar::setSpacing(int spacing)
{
    m_spacing = spacing;
    calc_buttons_positions();
    updateGeometry();
}

///
/// \brief isa_tool_bar::setContentsMargins
/// Отступы
/// \param left
/// \param top
/// \param right
/// \param bottom
///
void isa_tool_bar::setContentsMargins(int left, int top, int right, int bottom)
{
    m_left_mergin   = left;
    m_top_mergin    = top;
    m_right_mergin  = right;
    m_bottom_mergin = bottom;

    calc_sizes();
    calc_buttons_positions();
    updateGeometry();
}

void isa_tool_bar::try_visible()
{
    if(m_buttons_menu->actions().size() == 0)
    {
        hide();
        return;
    }

    if( ! m_all_actions_menu_visible && (m_buttons.size() == 0 || ! m_quick_toolbar_visible))
        hide();
    else
        show();
}

void isa_tool_bar::calc_sizes()
{
    bool horizontal = m_direction == QBoxLayout::LeftToRight || m_direction == QBoxLayout::RightToLeft;

    setMinimumHeight(m_top_mergin + m_size.height() + m_bottom_mergin);
    setMinimumWidth(m_left_mergin + m_size.width() + m_right_mergin);

    QSizePolicy policy = sizePolicy();
    if(horizontal)
    {
        policy.setHorizontalPolicy(QSizePolicy::Preferred);
        policy.setVerticalPolicy(QSizePolicy::Minimum);

        setMaximumHeight(m_top_mergin + m_size.height() + m_bottom_mergin);
        setMaximumWidth(QWIDGETSIZE_MAX);
    }
    else
    {
        policy.setHorizontalPolicy(QSizePolicy::Minimum);
        policy.setVerticalPolicy(QSizePolicy::Preferred);

        setMaximumHeight(QWIDGETSIZE_MAX);
        setMaximumWidth(m_left_mergin + m_size.width() + m_right_mergin);
    }
    setSizePolicy(policy);
    updateGeometry();
}

void isa_tool_bar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    calc_buttons_positions();
}

QSize isa_tool_bar::sizeHint() const
{
    bool horizontal = m_direction == QBoxLayout::LeftToRight || m_direction == QBoxLayout::RightToLeft;

    int enabled_n = 0;
    for(auto it : m_buttons)
        if(it.action->isVisible())
            enabled_n++;


    if(horizontal)
    {
        int qb_length = m_quick_toolbar_visible ? enabled_n*(m_size.width()+m_spacing) : 0;
        return QSize((m_all_actions_menu_visible ? (m_size.width() + m_left_mergin + m_right_mergin) : 0 ) + qb_length,
                     m_size.height() + m_top_mergin + m_bottom_mergin);
    }
    else
    {
        int qb_length = m_quick_toolbar_visible ? enabled_n*(m_size.height()+m_spacing) : 0;
        return QSize(m_size.width() + m_left_mergin + m_right_mergin,
                     (m_all_actions_menu_visible ? (m_size.height() + m_top_mergin + m_bottom_mergin) : 0 ) + qb_length);
    }
}

void isa_tool_bar::on_action_changed()
{
//    calc_sizes();

    // Если была смена видимости
    calc_buttons_positions();
    updateGeometry();

//    QAction *act = dynamic_cast<QAction*>(sender());

//    if(act != nullptr)
//    {
//        for(auto it: m_buttons)
//        {
//            if(it.action == act)
//            {
//                it.button->setCheckable(act->isCheckable());
//                it.button->setToolTip(act->toolTip());
//                it.button->setText(act->text());
//                it.button->setIcon(act->icon());
//                it.button->setVisible(act->isVisible());
//                it.button->setEnabled(act->isEnabled());
//                it.button->setIconSize( QSize(m_size-6,m_size-6) );
//            }
//        }
//    }

}

void isa_tool_bar::on_action_destroyed(QObject *obj)
{
    QAction *act = dynamic_cast<QAction*>(obj);

    if(act != nullptr)
        removeToolAction(act);
}

void isa_tool_bar::calc_buttons_positions()
{
    auto const &s = size();

    bool horizontal = m_direction == QBoxLayout::LeftToRight || m_direction == QBoxLayout::RightToLeft;
    bool reverse = m_direction == QBoxLayout::RightToLeft || m_direction == QBoxLayout::BottomToTop;
    QSize item_size = m_size;

    int maxN;
    int step;
    if( horizontal )
        step = item_size.width() + m_spacing;
    else
        step = item_size.height() + m_spacing;

    if(horizontal)
        maxN = (s.width() - m_top_mergin - m_bottom_mergin + m_spacing)/step;
    else
        maxN = (s.height() - m_left_mergin - m_right_mergin + m_spacing)/step;

    maxN -=  m_all_actions_menu_visible ? 1 : 0;

    int x = m_left_mergin;
    int y = m_top_mergin;

    // Кнопка вызова меню со списком действий
    if(m_all_action_menu_first && m_all_actions_menu_visible)
    {
        m_show_full_buttons->move(x, y);
        m_show_full_buttons->resize(m_size);
        m_show_full_buttons->setIconSize( m_size-QSize(6,6) );
        m_show_full_buttons->show();

        if(horizontal)
            x += step;
        else
            y += step;
    }

    // Расчет позиций кнопок быстрого доступа
    int i = 0;
    int n = 0;
    if(m_quick_toolbar_visible)
    {
        for( ; n < maxN && i < m_buttons.size(); ++i)
        {
            int j = i;
            if(reverse)
                j = m_buttons.size() - i -1;

            auto it = m_buttons.at(j).button;

            if(m_buttons.at(j).action->isVisible())
            {
                it->move(x, y);

                it->resize(m_size);
                it->setIconSize( m_size-QSize(6,6) );
                it->show();

                if(horizontal)
                    x += step;
                else
                    y += step;
                n++;
            }
            else
                it->hide();
        }
    }

    // Скрытие не влезших кнопок
    for( ; i < m_buttons.size(); ++i)
    {
        int j = i;
        if(reverse)
            j = m_buttons.size() - i -1;

        m_buttons.at(j).button->hide();
    }

    // Кнопка вызова меню со списком действий
    if( ! m_all_action_menu_first && m_all_actions_menu_visible)
    {
        m_show_full_buttons->move(x, y);
        m_show_full_buttons->resize(m_size);
        m_show_full_buttons->setIconSize( m_size-QSize(6,6) );
        m_show_full_buttons->show();
    }
}

///
/// \brief isa_tool_bar::getAllActionMenuFirst
/// Положение кнопки со вызывающей меню со списком всех действий
/// \return
///
bool isa_tool_bar::getAllActionMenuFirst() const
{
    return m_all_action_menu_first;
}

///
/// \brief isa_tool_bar::setAllActionMenuFirst
/// Положение кнопки со вызывающей меню со списком всех действий
/// \param v true - Слева (Сверху); false - Справа (Снизу)
///
void isa_tool_bar::setAllActionMenuFirst(bool v)
{
    m_all_action_menu_first = v;
    calc_buttons_positions();
}

///
/// \brief isa_tool_bar::setAllActionButtonVisible
/// Отображать или нет кнопку вызова меню с подробным списком действий
/// \param b
///
void isa_tool_bar::setAllActionButtonVisible(bool b)
{
    m_all_actions_menu_visible = b;

    m_show_full_buttons->setVisible(b);

    calc_buttons_positions();
    try_visible();
    updateGeometry();
}

///
/// \brief isa_tool_bar::getAllActionButtonVisible
/// Отображать или нет кнопку вызова меню с подробным списком действий
/// \return
///
bool isa_tool_bar::getAllActionButtonVisible() const
{
    return m_all_actions_menu_visible;
}

///
/// \brief isa_tool_bar::setQuickToolBarVisible
/// Отображать или нет действия "быстрого доступа"
/// \param b
///
void isa_tool_bar::setQuickToolBarVisible(bool b)
{
    m_quick_toolbar_visible = b;

    calc_buttons_positions();
    try_visible();
    updateGeometry();
}

///
/// \brief isa_tool_bar::getQuickToolBarVisible
/// \return
///
bool isa_tool_bar::getQuickToolBarVisible() const
{
    return m_quick_toolbar_visible;
}

///
/// \brief isa_tool_bar::getBottomMergin
/// Нижний отступ
/// \return
///
int isa_tool_bar::getBottomMergin() const
{
    return m_bottom_mergin;
}

///
/// \brief isa_tool_bar::getRightMergin
/// Правый отступ
/// \return
///
int isa_tool_bar::getRightMergin() const
{
    return m_right_mergin;
}

///
/// \brief isa_tool_bar::getTopMergin
/// Верхний отступ
/// \return
///
int isa_tool_bar::getTopMergin() const
{
    return m_top_mergin;
}

///
/// \brief isa_tool_bar::getLeftMergin
/// Левый отступ
/// \return
///
int isa_tool_bar::getLeftMergin() const
{
    return m_left_mergin;
}

///
/// \brief isa_tool_bar::getSpacing
/// Расстояние между кнопками
/// \return
///
int isa_tool_bar::getSpacing() const
{
    return m_spacing;
}

///
/// \brief isa_tool_bar::setButtonSize
/// \param getButtonSize
///
void isa_tool_bar::setButtonSize( int btn_size )
{
    setButtonSize( QSize(btn_size, btn_size) );
}

///
/// \brief isa_tool_bar::getButtonSize
/// Размер кнопок
/// \return
///
QSize isa_tool_bar::getButtonSize() const
{
    return m_size;
}

///
/// \brief isa_tool_bar::setButtonSize
/// Размер кнопок
/// \param size
///
void isa_tool_bar::setButtonSize(QSize size)
{
    m_size = size;
    calc_sizes();
    calc_buttons_positions();
    updateGeometry();
}



