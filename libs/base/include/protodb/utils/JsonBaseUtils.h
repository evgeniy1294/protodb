#pragma once

#include <nlohmann/json.hpp>

class QString;
class QJsonValue;
class QJsonObject;
class QByteArray;

void from_json(const nlohmann::json& j, QString& str);
void from_json(const nlohmann::json& j, QJsonValue& value);
void from_json(const nlohmann::json& j, QJsonObject& qjson);
void from_json(const nlohmann::json& j, QByteArray& array);

void to_json(nlohmann::json& j, const QString& str);
void to_json(nlohmann::json& j, const QJsonValue& value);
void to_json(nlohmann::json& j, const QJsonObject& qjson);
void to_json(nlohmann::json& j, const QByteArray& array);

bool readFromFile( const QString& path, nlohmann::json& json );
bool writeToFile ( const QString& path, const nlohmann::json& json, int intend = -1 );

