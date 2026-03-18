#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <tuple>
#include <unordered_set>
#include "message.h"


  
class ChatMessage : public Message {
public:
    QString content; 
    // Constructors
    ChatMessage(const QString& content);
    // Convert to JSON
    QJsonObject toJson() const override;
    // Convert to QByteArray
    QByteArray toQByteArray() const override;
    // Static method to construct ChatMessage from QByteArray
    static std::shared_ptr<ChatMessage> fromQByteArray(const QByteArray& byteArray); 
    // Static method to construct ChatMessage from QJsonObject
    static std::shared_ptr<ChatMessage> fromQJsonObject(const QJsonObject& jsonObj); 
}; 

class ChatMessageFactory{
public:
    static std::shared_ptr<ChatMessage> createChatMessage(const QString& content) {
 
        return std::make_shared<ChatMessage>(content);
    } 
private: 

};

#endif // CHAT_MESSAGE_H












