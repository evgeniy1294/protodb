#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <nlohmann/json_fwd.hpp>

namespace protodb {

class BytecodeEncoderCreator;
class BytecodeEncoderAbstract;
class ConfigWidgetAbstract;

class PROTODB_DEV_EXPORT BytecodeEncoderFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit BytecodeEncoderFactory(QObject* parent = nullptr);
    ~BytecodeEncoderFactory() = default;

    static QPointer< BytecodeEncoderFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< BytecodeEncoderCreator >& creator);

    QList< QSharedPointer< BytecodeEncoderCreator > > getAllCreators() const;
    QSharedPointer< BytecodeEncoderCreator > getCreator( const QString& cid ) const;
    QSharedPointer< BytecodeEncoderCreator > operator[]( const QString& cid ) const;

    // ------------ [ Create new ] --------- //
    ConfigWidgetAbstract* createConfigWidget(QWidget* parent = nullptr) const;
    ConfigWidgetAbstract* createConfigWidget(const nlohmann::json& json, QWidget* parent = nullptr) const;
    ConfigWidgetAbstract* createConfigWidget(const QString &cid, QWidget* parent = nullptr) const;
    ConfigWidgetAbstract* createConfigWidget(const QString &cid, const nlohmann::json& json, QWidget* parent = nullptr) const;

    BytecodeEncoderAbstract* createEncoder(QObject* parent = nullptr) const;
    BytecodeEncoderAbstract* createEncoder(const nlohmann::json& json, QObject* parent = nullptr) const;
    BytecodeEncoderAbstract* createEncoder(const QString &cid, QObject* parent = nullptr) const;
    BytecodeEncoderAbstract* createEncoder(const QString &cid, const nlohmann::json& json, QObject* parent = nullptr) const;

private:
    Q_DISABLE_COPY( BytecodeEncoderFactory )
};

} // namespace protodb
