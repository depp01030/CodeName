#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QJsonObject> 
#include <QJsonDocument> 

enum class MessageType{
    Message,
    GameMessage,
    SystemMessage,
    ChatMessage,
    CommandMessage,
    ServerCommandMessage, 
    // LogoutMessage,
    // RegisterMessage,
    // ForgetPasswordMessage,
    // ChangePasswordMessage,
    // ChangeUsernameMessage,
};
class Message{
public:
    MessageType messageType ; 
    QString userID ;
    QString userIP ; 
    QString uuid ; 
    QDateTime timestamp ; 
    Message(MessageType messageType) : messageType(messageType) {}
    virtual QJsonObject toJson() const {
        QJsonObject jsonObj = {
            {"type", static_cast<int>(messageType)}, 
            {"uuid", uuid},
            {"userID", userID},
            {"userIP", userIP},
            {"timestamp", timestamp.toString()}
        };
        return jsonObj;
    }
    virtual QByteArray toQByteArray() const {
        QJsonDocument jsonDoc(toJson());
        return jsonDoc.toJson(); 
    }
 
};  
#endif // MESSAGE_H











