#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager {
public:
    static SessionManager& instance() {
        static SessionManager instance;
        return instance;
    }

    void setCurrentUserID(const QString& userID) { currentUserID = userID; }
    QString getCurrentUserID() const { return currentUserID; }

private:
    QString currentUserID;
    SessionManager() {}
    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;
};

#endif // SESSIONMANAGER_H
