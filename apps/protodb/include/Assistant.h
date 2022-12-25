#pragma once

#include <QProcess>
#include <QCoreApplication>

class QString;

namespace protodb {

class Assistant {

Q_DECLARE_TR_FUNCTIONS(Assistant)

public:
    static Assistant& instance();
    void showDocumentation(const QString& file);

private:
    Assistant() = default;
   ~Assistant();

    bool startAssistant();
    void showError(const QString &message);
    void finished(int exitCode, QProcess::ExitStatus status);

    QScopedPointer<QProcess> m_process;
};

} // namespace protodb
