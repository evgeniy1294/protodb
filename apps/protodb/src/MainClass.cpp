#include "MainClass.h"
#include "SequenceModel.h"
#include "Logger.h"
#include "LogPrinter.h"

#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/factories/IODeviceFactory.h>
#include <protodb/factories/ScriptInterfaceFactory.h>
#include <protodb/creators/ScriptInterfaceCreator.h>
#include <protodb/ScriptInterface.h>

MainClass::MainClass()
    : m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
    , m_logger(new Logger(this))
    , m_log_printer(new LogPrinter(this))
{
    init_factories();
    m_incoming_sequences->setIncomingMode();
    m_outgoing_sequences->setOutgoingMode();
}

MainClass::~MainClass()
{
}

void MainClass::init_factories()
{
    if (!IOWidgetFactory::globalInstance()) {
        GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory);
    }

    if (!IODeviceFactory::globalInstance()) {
        GlobalFactoryStorage::addFactory(IODeviceFactory::fid(), new IODeviceFactory);
    }

    // Script Interfaces
    {
        if (!ScriptInterfaceFactory::globalInstance()) {
            GlobalFactoryStorage::addFactory(ScriptInterfaceFactory::fid(), new ScriptInterfaceFactory);
        }

        auto factory  = ScriptInterfaceFactory::globalInstance();
        auto creators = PluginManager::instance().getPlugins<ScriptInterfaceCreator>();

        for (auto& it: creators) {
            factory->addCreator(QSharedPointer<ScriptInterfaceCreator>(it));
        }
    }
}

MainClass &MainClass::instance()
{
    static MainClass m_instance;
    return m_instance;
}

void MainClass::init()
{
    // Create script interfaces
    m_script_interfaces.clear();
        auto script_interface_creators = ScriptInterfaceFactory::globalInstance()->getAllCreators();
        for (auto& creator: script_interface_creators) {
            m_script_interfaces.append( creator->create() );
        }

    // Create base supported syntax list
    m_supported_syntaxes.clear();
        m_supported_syntaxes.append("ascii");
        m_supported_syntaxes.append("hex");

    // Add script interface to supported syntax list
    for (auto& script_interface: m_script_interfaces) {
        m_supported_syntaxes.append(script_interface->syntaxId());
    }


    m_log_printer->setLogFile("/tmp/protodb.log");
    m_log_printer->setAppendFile(true);
    connect(m_logger, &Logger::sEventOccuaried, m_log_printer, &LogPrinter::print);
}

SequenceModel* MainClass::incomingSequences() const
{
    return m_incoming_sequences;
}

SequenceModel* MainClass::outgoingSequences() const
{
    return m_outgoing_sequences;
}

Logger* MainClass::logger() const
{
    return m_logger;
}

bool MainClass::isStarted() const
{
    return m_io != nullptr;
}

void MainClass::start(const nlohmann::json &attr)
{
    if (!m_log_printer->setEnabled()) {
        m_logger->error(QString("Can't open file: %1").arg(m_log_printer->logFile()).toLatin1());
    }
    emit sStarted();
}

void MainClass::stop()
{
    m_log_printer->setDisabled();
    emit sStopted();
}

QStringList MainClass::supportedSyntaxes() const
{
    return m_supported_syntaxes;
}
