#include "MainClass.h"
#include "SequenceModel.h"
#include "Logger.h"
#include "LogPrinter.h"
#include "LogFormatter.h"
#include "SequenceScriptFormatter.h"
#include "ScriptInterfaceGroup.h"

#include <protodb/plugins/PluginManager.h>
#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/factories/IOWidgetFactory.h>
#include <protodb/factories/IODeviceFactory.h>
#include <protodb/creators/IOWidgetCreatorInterface.h>
#include <protodb/creators/IODeviceCreatorInterface.h>
#include <protodb/factories/ScriptInterfaceFactory.h>
#include <protodb/creators/ScriptInterfaceCreator.h>
#include <protodb/ScriptInterface.h>

#include <QTimer>
#include <QRegularExpression>
#include <QApplication>

using namespace protodb;

MainClass::MainClass()
    : m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
    , m_logger(new Logger(this))
    , m_log_printer(new LogPrinter(this))
    , m_script_multi_interface(new ScriptMultiInterface(this))
    , m_guard_timer(new QTimer(this))
{
    m_guard_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_guard_timer->setSingleShot(true);

    m_d1 = -1;
    m_d2 = -1;
    m_d_custom = false;

    m_incoming_sequences->setIncomingMode();
    m_outgoing_sequences->setOutgoingMode();
    connect_signals();
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
    m_log_printer->setLogFile (
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() +"protodb.log"
    );

    m_log_printer->setAppendFile(true);

    connect(m_logger, &Logger::sEventOccuaried, m_log_printer, &LogPrinter::print);

    connect(m_script_multi_interface, &ScriptMultiInterface::sPrint,
            m_logger, QOverload<const QString&>::of(&Logger::comment));

    connect(m_script_multi_interface, &ScriptMultiInterface::sLogClear,
            m_logger, &Logger::clear);

    connect(m_script_multi_interface, &ScriptMultiInterface::sErrorOccuared,
            m_logger, QOverload<const QString&>::of(&Logger::error));

    connect(m_script_multi_interface, &ScriptMultiInterface::sStopSession,
            this, &MainClass::stop);
}

void MainClass::init_syntaxes()
{
    // Create script interfaces
        auto script_interface_creators = ScriptInterfaceFactory::globalInstance()->getAllCreators();
        for (auto& creator: script_interface_creators) {
            auto interface = QSharedPointer<ScriptInterface>(creator->create());
                m_script_multi_interface->addScriptInterface( interface );
        }

    // Add script interface to supported syntax list
    for (auto& script_interface: m_script_multi_interface->scriptInterfaces()) {
        auto formatter = qSharedPointerCast<SequenceFormatter> (
            QSharedPointer<SequenceScriptFormatter>::create(script_interface)
        );

        Sequence::addFormatter( formatter );
    }
}

void MainClass::connect_signals()
{
    connect(m_outgoing_sequences, &SequenceModel::sSequenceActivated, this, &MainClass::sequenceActivated);
    connect(m_outgoing_sequences, &SequenceModel::sSequenceDisactivated, this, &MainClass::sequenceDisactivated);
    connect(m_incoming_sequences, &SequenceModel::sSequenceDisactivated, this, &MainClass::sequenceDisactivated);

    connect(m_guard_timer, &QTimer::timeout, this, [this]() {
        read_message(m_buffer);
        m_buffer.clear();
    });
}

void MainClass::config_logger(const nlohmann::json &json)
{
    bool ch1_en = json.value<bool>("FirstChannelEnabled", true);
        if (ch1_en) {
            m_logger->setChannelEnabled(Logger::ChannelFirst);
        }
        else {
            m_logger->setChannelDisabled(Logger::ChannelFirst);
        }

    bool ch2_en = json.value<bool>("SecondChannelEnabled", true);
        if (ch2_en) {
            m_logger->setChannelEnabled(Logger::ChannelSecond);
        }
        else {
            m_logger->setChannelDisabled(Logger::ChannelSecond);
        }

    bool append_log = json.value<bool>("Append", true);

    m_log_printer->setAppendFile(append_log);
        m_log_printer->formatter()->setTimeFormat(json.value("TimestampFormat", QString("hh:mm:ss.sss")));
        m_log_printer->setTimestampEnabled(json.value("TimestampEnabled", true));
}

bool MainClass::try_create_connection(const QString& cid, const nlohmann::json &cfg)
{
    bool ret = false;

    auto factory = IODeviceFactory::globalInstance();
    if (!factory) {
        GlobalFactoryStorage::addFactory(IODeviceFactory::fid(), new IODeviceFactory);
        factory = IODeviceFactory::globalInstance();
    }

    m_io = factory->createIODevice(cid, cfg, m_seance_desc);
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
                                arg(m_io->errorString()).toUtf8());
            return false;
        }

        connect(m_io, &QIODevice::readyRead, this, &MainClass::readyRead);
        connect(m_io, &QIODevice::aboutToClose, this, &MainClass::stop);

        if (!m_log_printer->setEnabled()) {
            m_logger->error(QString("Can't open file: %1").arg(m_log_printer->logFile()).toUtf8());
        }

        ret = true;
    }
    else {
        m_logger->error(QString("Can't create IODevice: %1").arg(cid).toUtf8());
    }

    return ret;
}

bool MainClass::send_sequence(QSharedPointer<const Sequence>& sequence)
{
    if (sequence.isNull()) {
        return false;
    }

    if (!isStarted()) {
        start(); if (!isStarted()) return false;
    }

    if (!m_io->isWritable()) {
        m_logger->error(tr("Can't write data to this IO Device. May be device is read only?"));
        return false;
    }

    auto bytes = sequence->bytes();
    if (bytes.isEmpty()) {
        m_logger->error(tr("Sequence \"%1\" is empty or have invalid format").arg(sequence->name()).toUtf8());
        return false;
    }

    sendBytes(bytes);

    return true;
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

void MainClass::setSeanceConfigs(const nlohmann::json& cfg)
{
    m_seance_cfg = cfg;
}

void MainClass::seanceConfigs(nlohmann::json& cfg) const
{
    cfg = m_seance_cfg;
}

void MainClass::start()
{
    if (m_seance_cfg.empty()) {
        m_logger->error(QString("Can't start seance: empty configs"));
        emit sStartFailed();
        return;
    }

    // Configure logger
    auto log_configs = m_seance_cfg.value("LogConfigs", nlohmann::json::object());
    config_logger(log_configs);

    auto log_file = m_seance_cfg.value("LogFile", QString());
    if (log_file.isEmpty()) {
        log_file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "protodb.log";
    }

    m_log_printer->setLogFile(log_file);

    // Configure delimiter
    auto delimeters_cfg = m_seance_cfg.value("Delimeters", nlohmann::json::object());
    m_d1 = delimeters_cfg.value("Delimeter1", -1);
    m_d2 = delimeters_cfg.value("Delimeter2", -1);
    m_d_custom = delimeters_cfg.value("UseCustom", false);

    int guard_interval = delimeters_cfg.value("GuardInterval", 0);
        m_guard_timer->setInterval(guard_interval);

    // Set script file
    auto script_file = m_seance_cfg.value("ScriptFile", QString());
    if (!m_script_multi_interface->setScriptFile(script_file)) {
        m_logger->error(QString("Script file \"%1\" unsupported").arg(script_file).toUtf8());
    }

    // Init IODevice
    auto cid = m_seance_cfg.value("SelectedCID", std::string("Null"));
    auto device_configs = m_seance_cfg.value("Configs", nlohmann::json::object());
    auto cfg = device_configs.value(cid, nlohmann::json::object());

    if (try_create_connection(QString::fromStdString(cid), cfg)) {
        m_script_multi_interface->handleEvent( ScriptInterface::Start );
        emit sStarted(m_seance_desc);
    }
    else {
        stop();
    }

    return;
}

void MainClass::stop()
{
    auto io = m_io; m_io = nullptr;
    if (io) {
        if (io->isOpen()) {
            disconnect(io); io->close(); io->deleteLater();
            m_script_multi_interface->handleEvent( ScriptInterface::Stop );
        }
    }

    auto timers = m_repeat_timers.keys();
        timers.append(m_singleshot_timers.keys());
            m_repeat_timers.clear();
            m_singleshot_timers.clear();
    for (int i = 0; i < timers.count(); i++) {
        killTimer(i);
    }

    m_outgoing_sequences->disableAll();
    m_log_printer->setDisabled();
    emit sStopted();
}

void MainClass::sendBytes(QByteArray& bytes)
{
    if (bytes.isEmpty()) {
        return;
    }

    if (!isStarted()) {
        start(); if (!isStarted()) return;
    }

    if (!m_io->isWritable()) {
        m_logger->error(tr("Can't write data to this IO Device. May be device is read only?"));
        return;
    }

    m_script_multi_interface->handleDataEvent( ScriptInterface::Transmit, bytes );
    m_io->write( bytes );
    m_logger->log(Logger::ChannelSecond, bytes);
}

void MainClass::sequenceActivated(QUuid uid)
{
    if (sender() == m_outgoing_sequences) {
        auto sequence = m_outgoing_sequences->getSequenceByUuid(uid);
        if (!sequence.isNull()) {
            if (send_sequence(sequence) && sequence->active() && sequence->period() != 0) {
                auto timer_id = startTimer(sequence->period(), Qt::PreciseTimer);

                if (timer_id > 0) {
                    m_repeat_timers[timer_id] = sequence->uuid();
                }
                else { qDebug() << "Can't create timer"; }
            }
        }
    }

    return;
}

void MainClass::sequenceDisactivated(QUuid uid)
{
    int timer_id = sender() == m_outgoing_sequences ?
                        m_repeat_timers.key(uid, -1) : m_singleshot_timers.key(uid, -1);

    if (timer_id > 0) {
        killTimer(timer_id);
        m_repeat_timers.remove(timer_id);
    }
}

void MainClass::timerEvent(QTimerEvent* event)
{
    QSharedPointer<const Sequence> sequence;
    auto timer_id = event->timerId();

    if (!isStarted()) {
        killTimer(timer_id);
        return;
    }

    // This is repeat timers?
    auto suid = m_repeat_timers.value(timer_id, QUuid());
    if (!suid.isNull()) {
        sequence = m_outgoing_sequences->getSequenceByUuid(suid);
    }
    else {
        killTimer(timer_id);
        suid = m_singleshot_timers.value(timer_id, QUuid());

        if (suid.isNull()) {
            return;
        }

        auto incoming = m_incoming_sequences->getSequenceByUuid(suid);
        if (incoming.isNull()) {
            return;
        }

        sequence = m_outgoing_sequences->getSequenceByName( incoming->bindedName() );
    }

    send_sequence(sequence);

    return;
}

void MainClass::readyRead()
{
    m_buffer.append(m_io->readAll());
    if (m_buffer.isEmpty())
        return;

    m_guard_timer->stop();

    auto slicing = [this]() -> QList<QByteArray> {
        QList<QByteArray> ret;

        if (m_d_custom) {
            // Call script function
        }
        else {
            if (m_d1 != -1) {
                int count = 0; int index = count;

                while(count == index) {
                    int idx1 = m_buffer.indexOf(static_cast<char>(m_d1), count);
                    if (idx1 == -1) break;

                    if (m_d2 != -1) {
                        int idx2 = m_buffer.indexOf(static_cast<char>(m_d2), idx1);
                        index = idx2 == (idx1+1) ? idx2 + 1 : idx2;
                    }
                    else {
                        index = idx1 == -1 ? idx1 : idx1 + 1;
                    }

                    if (index != -1) {
                        ret.append(m_buffer.mid(count, index - count));
                        count = index;
                    }
                }

                if (count != 0) {
                    m_buffer.remove(0, count);
                }
            }
            else {
                ret.append(m_buffer);
                m_buffer.clear();
            }
        }

        return ret;
    };

    for (auto& msg: slicing()) {
        read_message(msg);
    }

    if (m_guard_timer->interval() != 0) {
        if (!m_buffer.isEmpty()) {
            m_guard_timer->start();
        }
    }

    return;
}

void MainClass::read_message(QByteArray msg)
{
    if (msg.isEmpty()) return;

    m_logger->log(Logger::ChannelFirst, msg);
    m_script_multi_interface->handleDataEvent(ScriptInterface::Received, msg);

    auto incoming = m_incoming_sequences->getSequenceByBytes(msg, true);
    if (!incoming.isNull()) {
        if ( incoming->period() <= 0 ) {
            auto sequence = m_outgoing_sequences->getSequenceByName(incoming->bindedName());
            send_sequence(sequence);
        }
        else {
            auto timer_id = startTimer(incoming->period(), Qt::PreciseTimer);
            m_singleshot_timers[timer_id] = incoming->uuid();
        }
    }
}
