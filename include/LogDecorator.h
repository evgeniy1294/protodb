#pragma once

#include <QtCore>
#include <QColor>
#include <QFont>
#include "Configurable.h"
#include "LogTableModel.h"



class LogDecorator: public QObject, public Configurable
{
    Q_OBJECT

public:
    LogDecorator(QObject* parent = nullptr);

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(LogTableModel::Channel channel, const QColor& color);
    QColor channelColor(LogTableModel::Channel channel) const;

    void setChannelFont(LogTableModel::Channel channel, const QFont& font);
    QFont channelFont(LogTableModel::Channel channel) const;

    void defaultConfig(nlohmann::json &json) const override;
    void fromJson(nlohmann::json &json) override;
    void toJson(nlohmann::json &json) const override;

signals:
    void sConfigChanged();

private:
    QColor m_attr_color;
    QFont  m_attr_font;
    QMap<LogTableModel::Channel, QColor> m_ch_colors;
    QMap<LogTableModel::Channel, QFont>  m_ch_fonts;
};
