#include "utils/JsonBaseUtils.h"

#include <QJsonArray>

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

bool writeToFile ( const QString& path, const nlohmann::json& json )
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(json.dump(4).c_str());
    file.close();

    return true;
}

