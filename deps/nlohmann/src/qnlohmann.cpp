#include <QColor>
#include <QFont>
#include <protodb/nlohmann/qnlohmann.h>

void to_json(nlohmann::json& j, const QString& str) {
    j = str.toStdString();
}

void from_json(const nlohmann::json& j, QString& str) {
    str = j.get<std::string>().c_str();
}

void to_json(nlohmann::json& j, const QColor& color) {
    j = color.name();
}

void from_json(const nlohmann::json& j, QColor& color) {
    color.setNamedColor(j.get<QString>());
}

void to_json(nlohmann::json& j, const QFont& font) {
    j = font.toString();
}

void from_json(const nlohmann::json& j, QFont& font) {
    font.fromString(j.get<QString>());
}

void toQJsonObject(QJsonObject& to, const nlohmann::json& from)
{
    for (const auto& [key, val]: from.items()) {
        to[key.c_str()] = toQJsonValue(val);
    }
}

void fromQJsonObject(nlohmann::json& to, const QJsonObject& from)
{
    for (const auto& key: from.keys()) {
        to[key.data()] = toNlohmannJson(from[key]);
    }
}

QJsonValue toQJsonValue(const nlohmann::json& json)
{
    switch(json.type()) {
        case nlohmann::json::value_t::array: {
            QJsonArray array;
            for (auto& it: json) {
                array.append( toQJsonValue(it) );
            }

            return array;
        }

        case nlohmann::json::value_t::object:
            return QJsonObject();

        case nlohmann::json::value_t::string:
            return QJsonValue( json.get<QString>() );

        case nlohmann::json::value_t::boolean:
            return QJsonValue( json.get<bool>() );

        case nlohmann::json::value_t::number_integer:
            return QJsonValue( json.get<int>() );

        case nlohmann::json::value_t::number_unsigned:
            return QJsonValue( json.get<qint64>() );

        case nlohmann::json::value_t::number_float:
            return QJsonValue( json.get<double>() );

        case nlohmann::json::value_t::null:
            [[fallthrough]];
        case nlohmann::json::value_t::discarded:
            [[fallthrough]];
        case nlohmann::json::value_t::binary:
            [[fallthrough]];
        default:
            break;
    }

    return QJsonValue();
}

void toNlohmannJson(nlohmann::json& json, const QJsonValue& value)
{

}
