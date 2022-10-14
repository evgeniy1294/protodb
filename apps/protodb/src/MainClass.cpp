#include "MainClass.h"
#include "SequenceModel.h"
#include "Logger.h"
#include "LogPrinter.h"

#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/factories/IODeviceFactory.h>
#include <protodb/creators/IOWidgetCreatorInterface.h>
#include <protodb/creators/IODeviceCreatorInterface.h>
#include <protodb/factories/ScriptInterfaceFactory.h>
#include <protodb/creators/ScriptInterfaceCreator.h>
#include <protodb/ScriptInterface.h>

#include <QRegularExpression>

MainClass::MainClass()
    : m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
    , m_logger(new Logger(this))
    , m_log_printer(new LogPrinter(this))
{
    m_incoming_sequences->setIncomingMode();
    m_outgoing_sequences->setOutgoingMode();
}

MainClass::~MainClass()
{
}

MainClass &MainClass::instance()
{
    static MainClass m_instance;
    return m_instance;
}

void MainClass::init()
{
    init_factories();
    init_syntaxes();
    init_logger();
}

void MainClass::init_factories()
{
    // IOWidget factories
    {
        if (!IOWidgetFactory::globalInstance()) {
            GlobalFactoryStorage::addFactory(IOWidgetFactory::fid(), new IOWidgetFactory);
        }

        auto factory  = IOWidgetFactory::globalInstance();
        auto creators = PluginManager::instance().getPlugins<IOWidgetCreator>();

        for (auto& it: creators) {
            factory->addCreator(QSharedPointer<IOWidgetCreator>(it));
        }
    }

    // IODevice factories
    {
        if (!IODeviceFactory::globalInstance()) {
            GlobalFactoryStorage::addFactory(IODeviceFactory::fid(), new IODeviceFactory);
        }

        auto factory  = IODeviceFactory::globalInstance();
        auto creators = PluginManager::instance().getPlugins<IODeviceCreator>();

        for (auto& it: creators) {
            factory->addCreator(QSharedPointer<IODeviceCreator>(it));
        }
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

void MainClass::init_logger()
{
    m_log_printer->setLogFile("/tmp/protodb.log");
    m_log_printer->setAppendFile(true);
    connect(m_logger, &Logger::sEventOccuaried, m_log_printer, &LogPrinter::print);
}

void MainClass::init_syntaxes()
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
}

void MainClass::config_logger(const nlohmann::json &json)
{
    bool ch1_en = log_configs.value<bool>("FirstChannelEnabled", true);
        if (ch1_en) {
            m_logger->setChannelEnabled(Logger::ChannelFirst);
        }
        else {
            m_logger->setChannelDisabled(Logger::ChannelFirst);
        }

    bool ch2_en = log_configs.value<bool>("SecondChannelEnabled", true);
        if (ch2_en) {
            m_logger->setChannelEnabled(Logger::ChannelSecond);
        }
        else {
            m_logger->setChannelDisabled(Logger::ChannelSecond);
        }

    bool append_log = log_configs.value<bool>("Append", true);
    m_log_printer->setAppendFile(append_log);
}

void MainClass::create_iodevice(const nlohmann::json &json)
{

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

#include <iostream>

void MainClass::start(const nlohmann::json& attr)
{
    std::cout << attr.dump(4) << std::endl;

    auto factory = IODeviceFactory::globalInstance();
    if (!factory) {
        GlobalFactoryStorage::addFactory(IODeviceFactory::fid(), new IOWidgetFactory);
        factory = IODeviceFactory::globalInstance();
    }

    // Configure logger
    auto log_configs = attr.value("LogConfigs", nlohmann::json::object());
    config_logger(log_configs);

    // Init IODevice
    auto io_cfg = attr.value("IODevice", nlohmann::json::object());
    auto cid = io_cfg.value("CID", QString());
    auto cfg = io_cfg.value("Attributes", nlohmann::json::object());
    m_io = factory->createIODevice(cid, cfg);
    if (m_io) {
        if (cfg.value("OpenMode", QString()) == "Monitoring") {
            m_io->open(QIODevice::ReadOnly);
        }
        else {
            m_io->open(QIODevice::ReadWrite);
        }

        if (!m_io->isOpen()) {
            qDebug() << m_io->errorString();
            m_logger->error(QString("Can't open device: %2").
                                arg(m_io->errorString()).toLatin1());
            stop();
            return;
        }

        if (!m_log_printer->setEnabled()) {
            m_logger->error(QString("Can't open file: %1").arg(m_log_printer->logFile()).toLatin1());
        }

        emit sStarted();
    }
    else {
        m_logger->error(QString("Can't create IODevice: %1").arg(cid).toLatin1());
        stop();
    }

    return;
}

void MainClass::stop()
{
    auto io = m_io;
        m_io = nullptr;
        if (io) { io->close(); delete io; }

    m_log_printer->setDisabled();
    emit sStopted();
}

QStringList MainClass::supportedSyntaxes() const
{
    return m_supported_syntaxes;
}
