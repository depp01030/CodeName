#include "chat_message.h"
 


// ChatMessage definitions
ChatMessage::ChatMessage(const QString& content)
    : Message(MessageType::ChatMessage),
    content(content){}

QJsonObject ChatMessage::toJson() const {
    QJsonObject jsonObj = Message::toJson();
    jsonObj["content"] = content; 
    return jsonObj;
}

QByteArray ChatMessage::toQByteArray() const {
    QJsonDocument doc(toJson());
    return doc.toJson();
    }

std::shared_ptr<ChatMessage> ChatMessage::fromQByteArray(const QByteArray& byteArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObj = jsonDoc.object();
    return fromQJsonObject(jsonObj);
}

std::shared_ptr<ChatMessage> ChatMessage::fromQJsonObject(const QJsonObject& jsonObj) {
    QString content = jsonObj["content"].toString(); 
    std::shared_ptr<ChatMessage> chatMessage = std::make_shared<ChatMessage>(content);
    chatMessage->userID = jsonObj["userID"].toString();
    chatMessage->userIP = jsonObj["userIP"].toString();
    chatMessage->uuid = jsonObj["uuid"].toString();
    chatMessage->timestamp = jsonObj["timestamp"].toVariant().toDateTime();
    return chatMessage;
}
