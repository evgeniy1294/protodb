#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QWidget>

class QScrollArea;
class QFrame;
class QParallelAnimationGroup;
class QToolButton;

namespace protodb {

class PROTODB_GUI_EXPORT SpoilerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpoilerWidget(const QString & title = "", const int animationDuration = 300, QWidget *parent = 0);
    ~SpoilerWidget() = default;

    void setAnimationDuration(int duration);
    int animationDuration() const;

    void setContentLayout(QLayout* contentLayout);
    QLayout* contentLayout() const;

    void setTitle(const QString& title);
    QString title() const;

    void setExpanded(bool expand = true);
    void setCollapsed(bool collapse = true);
    bool isExpanded() const;

private:
    void createGui();
    void connectSignals();

private:
    QToolButton* m_toggle_button;
    QFrame* m_header_line;
    QParallelAnimationGroup* m_toggle_animation;
    QScrollArea* m_content_area;
    int m_animation_duration;
};

} // namespace protodb
