#pragma once

#include <protodb/creators/IODeviceCreatorInterface.h>

#include <QIODevice>

namespace protodb {

class Seance;

class SerialIoSeanceCreator: public QObject, public SeanceCreator
{
    Q_OBJECT
    Q_INTERFACES(protodb::SeanceCreator)
    Q_PLUGIN_METADATA(IID "protodb.SerialIoSeanceCreator" FILE "SerialIoSeanceCreator.json")

public:
    explicit SerialIoSeanceCreator() = default;
    ~SerialIoSeanceCreator() override = default;

    static QString creatorId() { return QString("SerialIoSeanceCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    Seance* create(QObject* parent = nullptr) const override;
    Seance* create(const nlohmann::json& cfg, QObject* parent = nullptr) const override;
};

} // namespace protodb
