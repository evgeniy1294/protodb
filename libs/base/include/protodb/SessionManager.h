#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QFileInfoList>

class SessionManager: public QAbstractTableModel {
    Q_OBJECT
    Q_DISABLE_COPY(SessionManager)

public:
    enum Columns {
        kColumnName        = 0,
        kColumnLastChanged = 1,
        kColumnDescription = 2,

        kColumnCount
    };

public:
    SessionManager(QObject* parent = nullptr);
   ~SessionManager() = default;

    // -------------- [ Session Manager Interface ] -------------- //
    // Установка рабочего каталога
    bool setWorkingDirectory(const QString& path); //

    // Сохранение данных в рабочий каталог
    bool saveCurrentState();

    // Создание новой сессии или копии другой сессии
    virtual bool createSession(const QString& name, const QString& description = QString(), const QString& origin = QString()); //

    // Удаление сессии
    virtual bool removeSession(const QString& name); //
    virtual bool removeSession(int id); //

    // Загрузка сессии
    bool loadSession(const QString& name);
    bool loadSession(int id);

    // Сохранение сессии
    bool saveCurrentSession();

    // Импорт сессии
    bool importSession(const QString& path);

    // Экспорт сессии
    bool exportSession(const QString& name, const QString& path);
    bool exportSession(int id, const QString& path);

    QString lastError() const;

    // ------------- [ Table Model Interface ] ---------------- //
    int rowCount(const QModelIndex& parent = QModelIndex()) const override; //
    int columnCount(const QModelIndex& parent = QModelIndex()) const override; //
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override; //
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override; //

signals:
    void sSessionsAdded(QStringList names);
    void sSessionsRemoved(QStringList names);
    void sSessionsAboutToBeRemoved( QStringList names );    

private:
    void markLoaded(int id);

protected:
    struct Session {
        QString name;
        QString description;
        QDateTime last_changed;
    };

    int findSessionByName(const QString& name) const;

protected:
    // Отвечает за непосредственную загрузку сессии, для каждого приложения пишется индивидуально
    virtual bool load_session(const QString& path_to_folder) = 0;

    // Отвечает за непосредственное сохранение сессии, для каждого приложения пишется индивидуально
    virtual bool save_session(const QString& path_to_folder) = 0;

protected:
    QString m_last_error;
    QString m_working_dir_path;
    QString m_last_session_name;
    int m_curr_session_id;
    QList<Session> m_sessions;
};
