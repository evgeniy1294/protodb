#include "MainClass.h"
#include "ProtodbSessionManager.h"
#include "SequenceModel.h"
#include "Logger.h"
#include "LogFormatter.h"

MainClass::MainClass()
    : m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
    , m_logger(new Logger(this))
    , m_log_formatter(new LogFormatter())
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

LogFormatter *MainClass::logFormatter() const
{
    return m_log_formatter;
}

bool MainClass::isStarted() const
{
    return m_io != nullptr;
}

void MainClass::start(const nlohmann::json &attr)
{

    emit sStarted();
}

void MainClass::stop()
{

    emit sStopted();
}


