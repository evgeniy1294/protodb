#pragma once

#include <protodb/ScriptInterface.h>

namespace protodb {

class ScriptMultiInterface final: public ScriptInterface
{
    Q_OBJECT

public:
    ScriptMultiInterface(QObject* parent = nullptr);

   ~ScriptMultiInterface() = default;

    void addScriptInterface(QSharedPointer<ScriptInterface>& interface);
    void rmScriptInterface(QSharedPointer<ScriptInterface>& interface);
    QList<QSharedPointer<ScriptInterface>> scriptInterfaces() const;

    QStringList fileExtensions() const;
    QString fileExtention() const override;

    bool setScriptFile(const QString& path) override;
    QString scriptFile() const override;
    bool isValid() const override;

    QString syntaxId() const override;

    // Custom event handler
    bool handleDataEvent(Event event, QByteArray& bytes) override;

private slots:
    void error_occuared(QString detail);
    void print(QString text);
    void log_clear();
    void session_stop();
    void send_bytes(QByteArray bytes);

private:
    QList<QSharedPointer<ScriptInterface>> m_script_interfaces;
    QString m_script_file;
    int m_interface_id;
};

} // namespace protodb
