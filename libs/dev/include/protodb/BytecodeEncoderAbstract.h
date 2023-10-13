#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <QObject>

#include <nlohmann/json_fwd.hpp>

namespace protodb {

class PROTODB_DEV_EXPORT BytecodeEncoderAbstract: public QObject {
    Q_OBJECT

public:
    explicit BytecodeEncoderAbstract(const QString& remoteTypeName, const QString& remoteMimeType,
                                            const QString& remoteClipboardMimeType = QString(),
                                            QObject* parent = nullptr);

    virtual ~BytecodeEncoderAbstract();

public: // API to be implemented
    virtual QString encodeData(const QByteArray& data) const = 0;

public:
    QString remoteTypeName() const;
    QString remoteMimeType() const;

    // the clipboard does not yet understand mimetype inheritance
    QString remoteClipboardMimeType() const;

    virtual void setConfig(const nlohmann::json& cfg) { return; }
    virtual void config(nlohmann::json& cfg) const { return; }

Q_SIGNALS:
    void configChanged();

protected:
    const QString m_remote_type_name;
    const QString m_remote_mime_type;
    const QString m_remote_clipboard_mime_type;
};

inline BytecodeEncoderAbstract::BytecodeEncoderAbstract(const QString& remoteTypeName,
                                                                      const QString& remoteMimeType,
                                                                      const QString& remoteClipboardMimeType,
                                                                      QObject* parent)
    : QObject(parent)
    , m_remote_type_name(remoteTypeName)
    , m_remote_mime_type(remoteMimeType)
    , m_remote_clipboard_mime_type(remoteClipboardMimeType.isEmpty() ? remoteMimeType : remoteClipboardMimeType) {}

inline BytecodeEncoderAbstract::~BytecodeEncoderAbstract() = default;

inline QString BytecodeEncoderAbstract::remoteTypeName() const { return m_remote_type_name; }
inline QString BytecodeEncoderAbstract::remoteMimeType() const { return m_remote_mime_type; }
inline QString BytecodeEncoderAbstract::remoteClipboardMimeType() const { return m_remote_clipboard_mime_type; }

} // namespace protodb
