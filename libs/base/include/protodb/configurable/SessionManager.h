#pragma once

#include <QObject>

class SessionManager: public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SessionManager)

public:
    static SessionManager& instance();

private:
    SessionManager(QObject* parent = nullptr);
};
