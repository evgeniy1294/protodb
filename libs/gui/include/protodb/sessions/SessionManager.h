#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <protodb/utils/JsonBaseUtils.h>

#include <QAbstractTableModel>
#include <QDateTime>
#include <QFileInfoList>

namespace protodb {

class PROTODB_GUI_EXPORT SessionManager: public QAbstractTableModel {
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
    bool setWorkingDirectory(const QString& path);

    // Сохранение данных в рабочий каталог
    bool saveCurrentState();

    // Проверка наличия сессии
    bool containsSession(const QString& name) const;
    int findSessionByName(const QString& name) const;

    // Создание новой сессии или копии другой сессии
    virtual QString createSession(const QString& name, const QString& description = QString(), const QString& origin = QString());

    // Переименование сессии
    bool renameSession(const QString& curr_name, const QString& new_name);
    bool renameSession(int id, const QString& name);

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
    virtual bool sessionSupported(const nlohmann::json& meta);

    // Экспорт сессии
    bool exportSession(const QString& name, const QString& path);
    bool exportSession(int id, const QString& path);

    // Пользовательские данные
    void userData(nlohmann::json& json) const;
    void setUserData(const nlohmann::json& json);

    QString lastError() const;

    // ------------- [ Table Model Interface ] ---------------- //
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

signals:
    void sSessionsAdded(QStringList names);
    void sSessionsRemoved(QStringList names);
    void sSessionsAboutToBeRemoved( QStringList names );    

private:
    void markLoaded(int id);
    QString getUniqueSessionName(const QString& base = QString());

protected:
    struct Session {
        QString name;
        QString description;
        QDateTime last_changed;
    };

protected:
    // Отвечает за непосредственную загрузку сессии, для каждого приложения пишется индивидуально
    virtual bool load_session(const QString& path_to_folder) = 0;

    // Отвечает за непосредственное сохранение сессии, для каждого приложения пишется индивидуально
    virtual bool save_session(const QString& path_to_folder) = 0;

protected:
    int m_curr_session_id;
    QString m_last_error;
    QString m_working_dir_path;
    QString m_current_session_name;
    QList<Session> m_sessions;

    nlohmann::json m_user_data;
};

} // namespace protodb
