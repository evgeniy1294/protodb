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

