#include "MainClass.h"
#include "ProtodbSessionManager.h"
#include "SequenceModel.h"

MainClass::MainClass() { }

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
    m_incoming_sequences = new SequenceModel(this);
        m_incoming_sequences->setIncomingMode();

    m_outgoing_sequences = new SequenceModel(this);
        m_outgoing_sequences->setOutgoingMode();
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

    emit sStarted();
}

void MainClass::stop()
{

    emit sStopted();
}


