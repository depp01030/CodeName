#ifndef USER_H
#define USER_H

#include <QString>
#include "game_state.h" 
class User : public QObject {
    Q_OBJECT  // 添加這行
public:
    // Static method to get the singleton instance
    static void setInstance(const QString& userID, const QString& userIP);
    static void setInstance(const QString& userIP);
    static User* getInstance();

    // Delete copy constructor and assignment operator
    User(const User&) = delete;
    User& operator=(const User&) = delete;
    void setUserID(const QString& userID){
        m_userID = userID;
        emit userIDChanged(m_userID);
    }
    QString getUserID() const {
        return m_userID;
    }
    void setUserIP(const QString& userIP){
        m_userIP = userIP;
        emit userIPChanged(m_userIP);   
    }
    QString getUserIP() const {
        return m_userIP;
    }
    void setTeamType(TeamType team){
        m_teamType = team;
        emit teamTypeChanged(m_teamType);
    }
    TeamType getTeamType() const {
        return m_teamType;
    }
    QString getStrTeamType() const {
        if (m_teamType == TeamType::Red) {
            return QString("Red");
        } else if (m_teamType == TeamType::Blue) {
            return QString("Blue");
        } else if (m_teamType == TeamType::God) {
            return QString("God");
        }
        return QString("Unknown");
    }
    void setUUID(const QString& uuid){
        m_uuid = uuid;
        emit uuidChanged(m_uuid);
    }
    QString getUUID() const {
        return m_uuid;
    }
 
private:
    QString m_userID;
    QString m_userIP;
    QString m_uuid;
    TeamType m_teamType;
    

    // Private constructor
    User(const QString& userID, const QString& userIP, QObject *parent = nullptr)
        : QObject(parent), m_userID(userID), m_userIP(userIP), m_teamType(TeamType::Unknown) {}

    // Static instance pointer
    static User* instance;
signals:
    void userIDChanged(const QString& userID);
    void userIPChanged(const QString& userIP);
    void teamTypeChanged(TeamType teamType);
    void uuidChanged(const QString& uuid);
};

#endif