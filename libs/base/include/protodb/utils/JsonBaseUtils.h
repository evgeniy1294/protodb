#pragma once

#include <nlohmann/json.hpp>
#include <QtCore>

void from_json(const nlohmann::json& j, QString& str);
void from_json(const nlohmann::json& j, QJsonValue& value);
void from_json(const nlohmann::json& j, QJsonObject& qjson);

void to_json(nlohmann::json& j, const QString& str);
void to_json(nlohmann::json& j, const QJsonValue& value);
void to_json(nlohmann::json& j, const QJsonObject& qjson);

bool readFromFile( const QString& path, nlohmann::json& json );
bool writeToFile ( const QString& path, const nlohmann::json& json );

