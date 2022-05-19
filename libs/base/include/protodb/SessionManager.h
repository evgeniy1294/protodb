#pragma once

#include <QAbstractTableModel>
#include <QDateTime>

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
    // -------------- [ Session Manager Interface ] -------------- //
    static SessionManager& instance();

    // Установка рабочего каталога
    bool setWorkingDirectory(const QString& path);

    // Сохранение модели
    virtual bool saveToWorkingDirectory();

    // ID загруженной сессии
    int currentSessionId() const;

    // Создание новой сессии или копии другой сессии
    virtual bool createSession(const QString& name, const QString& description = QString(), const QString& origin = QString());

    // Удаление сессии
    virtual bool removeSession(const QString& name);
    virtual bool removeSession(int id);

    // Загрузка сессии
    bool loadLastSession();
    virtual bool loadSession(const QString& name);
    virtual bool loadSession(int id);

    // Сохранение сессии
    virtual bool saveSession(const QString& name = QString());

    QString lastError() const;

    // ------------- [ Table Model Interface ] ---------------- //
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

signals:
    void sSessionsAdded(QStringList names);
    void sSessionsRemoved(QStringList names);
    void sSessionsAboutToBeRemoved( QStringList names );

private:
    struct Session {
        QString name;
        QString description;
        QDateTime last_changed;
    };

    SessionManager(QObject* parent = nullptr);
    int findSessionByName(const QString& name) const;

private:
    QString m_last_error;
    QString m_working_dir_path;
    QString m_last_session_name;
    int m_curr_session_id;
    QList<Session> m_sessions;
};
