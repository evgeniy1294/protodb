#pragma once

#include <QObject>

class SessionManager: public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SessionManager)

public:
    static SessionManager& instance();

    // Установка рабочего каталога
    bool setWorkingDirectory(const QString& path);

    // Имя текущей загруженной сессии
    QString currentSession() const;

    // Список сессий
    QStringList availableSessions() const;

    // Переименование сессии
    bool renameSession(const QString& old_name, const QString& new_name);

    // Создание новой сессии или копии другой сессии
    virtual bool createSession(const QString& name, const QString& origin = QString());

    // Удаление сессии
    bool removeSession(const QString& name);

    // Загрузка сессии
    bool loadLastSession();
    virtual bool loadSession(const QString& name);

    // Сохранение сессии
    virtual bool saveSession(const QString& name = QString());

signals:
    void sSessionAdded(QString name);
    void sSessionRemoved(QString name);
    void sSessionAboutToBeRemoved( const QString& prefix );

private:
    SessionManager(QObject* parent = nullptr);
};
