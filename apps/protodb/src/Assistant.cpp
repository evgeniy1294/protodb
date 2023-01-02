#include "Assistant.h"

#include <QApplication>
#include <QMessageBox>
#include <QLibraryInfo>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

using namespace protodb;

static QString documentationDirectory()
{
    QStringList paths;

    paths.append(QCoreApplication::applicationDirPath());
    paths.append(QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation));

    for (const auto &dir : qAsConst(paths)) {
        const QString path = dir + QLatin1String("/doc/protodb");
        if (QFileInfo::exists(path))
            return path;
    }

    return QString();
}

Assistant& Assistant::instance()
{
    static Assistant m_instance = Assistant();
    return m_instance;
}

Assistant::~Assistant()
{
    if (!m_process.isNull() && m_process->state() == QProcess::Running) {
        QObject::disconnect(m_process.data(),
                            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                            nullptr, nullptr);
        m_process->terminate();
        m_process->waitForFinished(3000);
    }
}

void Assistant::showDocumentation(const QString &page)
{
    if (!startAssistant())
        return;

    QByteArray ba("SetSource ");
    ba.append("qthelp://org.protodb.app/doc/");

    m_process->write(ba + page.toLocal8Bit() + '\n');
}

bool Assistant::startAssistant()
{
    // ?????
    if (m_process.isNull()) {
        m_process.reset(new QProcess());
        QObject::connect(m_process.data(),
                         QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                         m_process.data(), [this](int exitCode, QProcess::ExitStatus status) {
                             this->finished(exitCode, status);
                         });
    }

    if (m_process->state() != QProcess::Running) {
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
        #else
            QString app = QLibraryInfo::path(QLibraryInfo::BinariesPath) + QDir::separator();
        #endif

#ifndef Q_OS_DARWIN
        app += QLatin1String("assistant");
#else
        app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

        const QString collectionDirectory = documentationDirectory();
        if (collectionDirectory.isEmpty()) {
            showError(tr("The documentation directory cannot be found"));
            return false;
        }

        QStringList args{QLatin1String("-collectionFile"),
                         collectionDirectory + QLatin1String("/protodb.qhc"),
                         QLatin1String("-enableRemoteControl")};

        m_process->start(app, args);

        if (!m_process->waitForStarted()) {
            showError(tr("Unable to launch Qt Assistant (%1): %2").arg(app, m_process->errorString()));
            return false;
        }
    }
    return true;
}

void Assistant::showError(const QString& message)
{
    QMessageBox::critical(QApplication::activeWindow(), "protodb", message);
}

void Assistant::finished(int exitCode, QProcess::ExitStatus status)
{
    const QString stdErr = QString::fromLocal8Bit(m_process->readAllStandardError());
    if (status != QProcess::NormalExit) {
        showError(tr("Assistant crashed: ").arg(stdErr));
    } else if (exitCode != 0) {
        showError(tr("Assistant exited with %1: %2").arg(exitCode).arg(stdErr));
    }
}
