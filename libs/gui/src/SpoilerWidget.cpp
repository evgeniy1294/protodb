#include "SpoilerWidget.h"

#include <QFrame>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>

namespace protodb {

SpoilerWidget::SpoilerWidget(const QString & title, const int animationDuration, QWidget *parent)
    : QWidget(parent)
    , m_animation_duration(animationDuration)
{
    createGui();
        m_toggle_button->setText(title);

    connectSignals();
}

void SpoilerWidget::setAnimationDuration(int duration)
{
    m_animation_duration = duration;

    for (int i = 0; i < m_toggle_animation->animationCount() - 1; ++i) {
        QPropertyAnimation* spoilerAnimation = static_cast<QPropertyAnimation*>(m_toggle_animation->animationAt(i));
        spoilerAnimation->setDuration(m_animation_duration);
    }

    QPropertyAnimation* contentAnimation =
        static_cast<QPropertyAnimation*>(m_toggle_animation->animationAt(m_toggle_animation->animationCount() - 1));
    contentAnimation->setDuration(m_animation_duration);
}

int SpoilerWidget::animationDuration() const
{
    return m_animation_duration;
}

void SpoilerWidget::setContentLayout(QLayout* contentLayout) {
    delete m_content_area->layout();
    m_content_area->setLayout(contentLayout);

    const auto collapsedHeight = sizeHint().height() - m_content_area->maximumHeight();
    auto contentHeight = contentLayout->sizeHint().height();
    for (int i = 0; i < m_toggle_animation->animationCount() - 1; ++i) {
        QPropertyAnimation * spoilerAnimation = static_cast<QPropertyAnimation*>(m_toggle_animation->animationAt(i));
        spoilerAnimation->setDuration(m_animation_duration);
        spoilerAnimation->setStartValue(collapsedHeight);
        spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
    }

    QPropertyAnimation * contentAnimation =
        static_cast<QPropertyAnimation*>(m_toggle_animation->animationAt(m_toggle_animation->animationCount() - 1));
    contentAnimation->setDuration(m_animation_duration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);

    return;
}

QLayout* SpoilerWidget::contentLayout() const
{
    return m_content_area->layout();
}

void SpoilerWidget::setTitle(const QString& title)
{
    m_toggle_button->setText(title);
}

QString SpoilerWidget::title() const
{
    return m_toggle_button->text();
}

void SpoilerWidget::setExpanded(bool expand)
{
    if (expand) {
        m_toggle_button->setChecked(true);
        m_toggle_button->setArrowType(Qt::ArrowType::DownArrow);
        m_toggle_animation->setDirection(QAbstractAnimation::Forward);
        m_toggle_animation->start();
    }
    else {
        setCollapsed();
    }
}

void SpoilerWidget::setCollapsed(bool collapse)
{
    if (collapse) {
        m_toggle_button->setChecked(false);
        m_toggle_button->setArrowType(Qt::ArrowType::RightArrow);
        m_toggle_animation->setDirection(QAbstractAnimation::Backward);
        m_toggle_animation->start();
    }
    else {
        setExpanded();
    }
}

bool SpoilerWidget::isExpanded() const
{
    return m_toggle_button->isChecked();
}

void protodb::SpoilerWidget::createGui()
{
    m_toggle_button = new QToolButton;
        m_toggle_button->setStyleSheet("QToolButton { border: none; }");
        m_toggle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        m_toggle_button->setArrowType(Qt::ArrowType::RightArrow);
        m_toggle_button->setCheckable(true);
        m_toggle_button->setChecked(false);

    m_header_line = new QFrame;
        m_header_line->setFrameShape(QFrame::HLine);
        m_header_line->setFrameShadow(QFrame::Sunken);
        m_header_line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);


    m_content_area = new QScrollArea;
        m_content_area->setStyleSheet("QScrollArea { border: none; }");
        m_content_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_content_area->setMaximumHeight(0);
        m_content_area->setMinimumHeight(0);

    m_toggle_animation = new QParallelAnimationGroup(this);
        m_toggle_animation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
        m_toggle_animation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
        m_toggle_animation->addAnimation(new QPropertyAnimation(m_content_area, "maximumHeight"));

    auto m_main_layout = new QGridLayout;
        m_main_layout->setVerticalSpacing(0);
        m_main_layout->setContentsMargins(0, 0, 0, 0);
        m_main_layout->addWidget(m_toggle_button, 0, 0, 1, 1, Qt::AlignLeft);
        m_main_layout->addWidget(m_header_line, 1, 2, 1, 1);
        m_main_layout->addWidget(m_content_area, 1, 0, 1, 3);
        setLayout(m_main_layout);
}

void protodb::SpoilerWidget::connectSignals()
{
    connect(m_toggle_button, &QToolButton::clicked, this, [this](const bool checked) {
        m_toggle_button->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        m_toggle_animation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        m_toggle_animation->start();
    });
}

} // namespace protodb
