#ifndef SYSTEM_MESSAGE_H
#define SYSTEM_MESSAGE_H

#include <QString>
#include <QJsonObject>
#include "message.h"
 
enum class SystemCommandType{
    None,
    CreateGame,
    InitGame,
    Start,
    Help,
    Unknown,
    SetUserUUID,
    SetUserID,
    SetUserTeam,
    SetUserRole,
    UserLogin,
    UserLogout,
    SetPlayerToTeam,
    AddPlayer,
};

class SystemMessage : public Message
{
public:
    SystemCommandType systemCommandType;
    QString content;
    SystemMessage(SystemCommandType systemCommandType, const QString& content);
public:
    QJsonObject toJson() const override;
    QByteArray toQByteArray() const override;
    static std::shared_ptr<SystemMessage> fromQByteArray(const QByteArray& byteArray);
    static std::shared_ptr<SystemMessage> fromQJsonObject(const QJsonObject& jsonObj);

};
class SystemMessageFactory{
private:
    static const std::unordered_map<QString, SystemCommandType> COMMANDS_MAP;
public:
    static bool isCommand(const QString& content);
    static std::tuple<QString, SystemCommandType> getCommand(const QString& content);
    static std::shared_ptr<SystemMessage> createSystemMessage(const QString& content);
};

#endif // SYSTEM_MESSAGE_H  