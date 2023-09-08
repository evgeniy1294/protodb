#pragma once

#include <protodb/export/base_cfg.hpp>

#include <nlohmann/json_fwd.hpp>

class QString;
class QJsonValue;
class QJsonObject;
class QByteArray;

namespace protodb {
    struct CrcModel;
    void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const CrcModel& model);
    void PROTODB_BASE_EXPORT from_json(const nlohmann::json& j, CrcModel& model);
}

void PROTODB_BASE_EXPORT from_json(const nlohmann::json& j, QString& str);
void PROTODB_BASE_EXPORT from_json(const nlohmann::json& j, QJsonValue& value);
void PROTODB_BASE_EXPORT from_json(const nlohmann::json& j, QJsonObject& qjson);
void PROTODB_BASE_EXPORT from_json(const nlohmann::json& j, QByteArray& array);

void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const QString& str);
void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const QJsonValue& value);
void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const QJsonObject& qjson);
void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const QByteArray& array);
void PROTODB_BASE_EXPORT to_json(nlohmann::json& j, const QByteArray& array);

bool PROTODB_BASE_EXPORT readFromFile( const QString& path, nlohmann::json& json );
bool PROTODB_BASE_EXPORT writeToFile ( const QString& path, const nlohmann::json& json, int intend = -1 );

