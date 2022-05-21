#pragma once

#include <protodb/utils/JsonBaseUtils.h>
#include <QColor>
#include <QFont>

void to_json(nlohmann::json& j, const QColor& color);
void to_json(nlohmann::json& j, const QFont& font);

void from_json(const nlohmann::json& j, QColor& color);
void from_json(const nlohmann::json& j, QFont& font);
