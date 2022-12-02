#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <protodb/utils/JsonBaseUtils.h>
#include <QColor>
#include <QFont>

void PROTODB_GUI_EXPORT to_json(nlohmann::json& j, const QColor& color);
void PROTODB_GUI_EXPORT to_json(nlohmann::json& j, const QFont& font);

void PROTODB_GUI_EXPORT from_json(const nlohmann::json& j, QColor& color);
void PROTODB_GUI_EXPORT from_json(const nlohmann::json& j, QFont& font);
