#pragma once

#include <protodb/configurable/Configurable.h>

class ConfigAggregator:
    public QObject,
    public Configurable
{
    Q_OBJECT

public:
    bool attachObject(Configurable* cfg);      // Подключение объекта к аггрегатору
    void detachObject(Configurable* cfg);      // Отключение объекта от аггрегатора
    void detachObject(const QString& prefix);  // Отключение объекта от аггрегатора по префиксу

    void setObjectConfig(const QString& prefix, const nlohmann::json& json);
    void getObjectConfig(const QString& prefix, nlohmann::json& json);
    void modifyObjectConfig(const QString& prefix, const nlohmann::json& json);


    void clear(); // Очистка хранилища настроек

private:

};
