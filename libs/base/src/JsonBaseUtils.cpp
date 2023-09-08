#include "utils/JsonBaseUtils.h"
#include "utils/crc_logic.h"

#include <nlohmann/json.hpp>

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

void to_json(nlohmann::json& j, const QString& str) {
    j = str.toStdString();
}

void from_json(const nlohmann::json& j, QString& str) {
    str = j.get<std::string>().c_str();
}


void to_json(nlohmann::json& j, const QJsonValue& value)
{
    switch(value.type()) {
        case QJsonValue::Bool:
            j = value.toBool();
            break;

        case QJsonValue::Double:
            j = value.toDouble();
            break;

        case QJsonValue::String:
            j = value.toString();
            break;

        case QJsonValue::Array: {
            const auto& array = value.toArray();
            for (const auto& it: array) {
                j.push_back(it);
            }
        } break;

        case QJsonValue::Object: {
            j = value.toObject();
        } break;

        case QJsonValue::Undefined:
            j = nlohmann::json(nlohmann::json::value_t::object);
            break;

        case QJsonValue::Null:
            Q_FALLTHROUGH();
        default:
            j = nlohmann::json();
            break;
    }

    return;
}

void from_json(const nlohmann::json& j, QJsonValue& value)
{
    switch(j.type()) {
        case nlohmann::json::value_t::array: {
            QJsonArray ret;
            for (auto& it: j) { ret.append( it ); }

            value = ret;
        } break;

        case nlohmann::json::value_t::object: {
            QJsonObject ret = j;
            value = ret;
        } break;

        case nlohmann::json::value_t::string:
            value = j.get<QString>();
            break;

        case nlohmann::json::value_t::boolean:
            value = j.get<bool>();
            break;

        case nlohmann::json::value_t::number_integer:
            value = j.get<int>();
            break;

        case nlohmann::json::value_t::number_unsigned:
            value = j.get<qint64>();
            break;

        case nlohmann::json::value_t::number_float:
            value = j.get<double>();
            break;

        case nlohmann::json::value_t::binary:
            value = QJsonValue(QJsonValue::Object);
            break;

        case nlohmann::json::value_t::null:
            Q_FALLTHROUGH();
        case nlohmann::json::value_t::discarded:
            Q_FALLTHROUGH();
        default:
            value = QJsonValue(QJsonValue::Null);
            break;
    }

    return;
}

void from_json(const nlohmann::json& j, QJsonObject& qjson)
{
    for ( const auto& [key, val]: j.items() ) {
        qjson[key.c_str()] = val;
    }
}

void to_json(nlohmann::json& j, const QJsonObject& qjson)
{
    for (auto& key: qjson.keys()) {
        j[key.toStdString()] = qjson[key];
    }
}

bool readFromFile( const QString& path, nlohmann::json& json )
{
    if (!QFile::exists(path)) {
        return false;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    json = nlohmann::json::parse(file.readAll().toStdString(), nullptr, false);
    if (json.is_discarded()) {
        qDebug() << "readFromFile:: json discarted";
        return false;
    }

    return true;
}

bool writeToFile ( const QString& path, const nlohmann::json& json, int intend )
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(json.dump(intend).c_str());
    file.close();

    return true;
}


void to_json(nlohmann::json& j, const QByteArray& array)
{
    j = nlohmann::json::array();
    for (auto& byte: qAsConst(array)) {
        j.push_back(static_cast<uint8_t>(byte));
    }
}

void from_json(const nlohmann::json& j, QByteArray& array)
{
    array.clear();

    if (!j.is_array()) {
        return;
    }

    array.resize(static_cast<int>(j.size()));
    for(int i = 0; i < j.size(); i++) {
        array[i] = j[i].get<std::uint8_t>();
    }
}

namespace protodb {

void from_json(const nlohmann::json& j, CrcModel& model)
{
    model.width   = j.value("Width", 16);
    model.poly    = j.value("Poly", uint64_t(0x8005u));
    model.seed    = j.value("Seed", uint64_t(0xffffu));
    model.xor_out = j.value("XorOut", uint64_t(0x0000u));
    model.ref_in  = j.value("ReflectIn", true);
    model.ref_out = j.value("ReflectOut", true);
}

void to_json(nlohmann::json& j, const CrcModel& model)
{
    j["Width"]      = model.width;
    j["Poly"]       = model.poly;
    j["Seed"]       = model.seed;
    j["XorOut"]     = model.xor_out;
    j["ReflectIn"]  = model.ref_in;
    j["ReflectOut"] = model.ref_out;
}

} // namespace protodb
