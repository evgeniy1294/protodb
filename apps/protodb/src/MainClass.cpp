#include "MainClass.h"
#include "ProtodbSessionManager.h"
#include "SequenceModel.h"
#include "Logger.h"
#include "LogFormatter.h"
#include "LogPrinter.h"

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


