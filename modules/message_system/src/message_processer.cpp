#include "network_data_manager.h"
#include "game_state.h"

#include "message.h"
#include "game_message.h"
#include "chat_message.h" 
#include "system_message.h"
#include "message_processer.h"
#include "user.h"
MessageProcesser::MessageProcesser(NetworkDataManager *networkDataManager):
    m_networkDataManager(networkDataManager){ 
 
    }

std::vector<std::shared_ptr<Message>> MessageProcesser::processSocketMessage(const QByteArray& data){
    std::vector<std::shared_ptr<Message>> messages;
    QString jsonString(data);  
    QStringList jsonStrings = jsonString.split("\end");

    for (const QString& str : jsonStrings) {
        // 如果字串不是空的
        if (!str.trimmed().isEmpty()) {
            // 嘗試解析 JSON
            QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
            if (!doc.isNull()) {
                QJsonObject messageObject = doc.object(); 
                std::shared_ptr<Message> message = jsonToMessage(messageObject);
                if (message != nullptr){
                    messages.push_back(message);
                }
            }
        }
    } 
    return messages;
}
std::shared_ptr<Message> MessageProcesser::jsonToMessage(const QJsonObject& messageObject) {
    std::shared_ptr<Message> message;
    MessageType messageType = static_cast<MessageType>(messageObject["type"].toInt());

    switch (messageType) {
        case MessageType::ChatMessage:
            message = ChatMessage::fromQJsonObject(messageObject);
            break;

        case MessageType::GameMessage:
            message = GameMessage::fromQJsonObject(messageObject); 
            break;
        case MessageType::SystemMessage:
            message = SystemMessage::fromQJsonObject(messageObject);
            break;
        default:
            // 处理未知的 MessageType，可能需要记录日志或抛出异常
            qWarning() << "jsonToMessage : Unknown MessageType:" << static_cast<int>(messageType);
            break;
    }

    return message;
}

std::shared_ptr<Message> MessageProcesser::messageWrapper(std::shared_ptr<Message> message) {
    try { 
        message->userID = User::getInstance()->getUserID();
        message->uuid = User::getInstance()->getUUID();
        message->userIP = User::getInstance()->getUserIP();
        message->timestamp = QDateTime::currentDateTime();
 
        
        return message;
    } catch (const std::exception& e) {
        qCritical() << "Error in messageWrapper: " << e.what();
        return nullptr; // Return an empty QJsonObject in case of error
    }
}
QByteArray MessageProcesser::messageObjToQByteArray(std::shared_ptr<Message> message) {
    // QByteArray byteMessage = QJsonDocument(message->toJson()).toJson();
    QByteArray byteMessage = message->toQByteArray();
 
    byteMessage.append("\end");
    return byteMessage;
}
bool MessageProcesser::needBroadcast(std::shared_ptr<Message> message){
    if (message->messageType == MessageType::SystemMessage){
        std::shared_ptr<SystemMessage> systemMessage = std::dynamic_pointer_cast<SystemMessage>(message);
        if (systemMessage->systemCommandType == SystemCommandType::CreateGame){
            return false;
        }
    }
    else if  (message->messageType == MessageType::GameMessage){
        std::shared_ptr<GameMessage> gameMessage = std::dynamic_pointer_cast<GameMessage>(message);
        if (gameMessage->operationData->operation == Operation::ClickCard){
            return false;
        }
    }
    return true;
}
void MessageProcesser::handleMessage(std::shared_ptr<Message> message)
{ 
    qDebug() << "MessageProcesser::handleMessage";
    qDebug() << "message : " << message->toJson();
    MessageType messageType = message->messageType;
    switch(messageType){
        case MessageType::ChatMessage: 
            handleChatMessage(message); 
            break;
        case MessageType::GameMessage:
            handleGameMessage(message);
            break;
        case MessageType::SystemMessage:
            handleSystemMessage(message);
            break;
        case MessageType::CommandMessage: 
            handleCommandMessage(message); 
            break;
        case MessageType::ServerCommandMessage: 
            handleCommandMessage(message);
            break;
        default:
            qWarning() << "Unknown MessageType:" << static_cast<int>(messageType);
            break;
    }
}
void MessageProcesser::handleChatMessage(std::shared_ptr<Message> message){ 
    // parse message here
    qDebug() << "MessageProcesser::handleChatMessage";

    emit m_networkDataManager->receiveChatMessageFromServer(message);   
    
    
}
void MessageProcesser::handleCommandMessage(std::shared_ptr<Message> message){
    qDebug() << "MessageProcesser::handleCommandMessage";

    
}

void MessageProcesser::handleGameMessage(std::shared_ptr<Message> message){ 
    // 
    emit m_networkDataManager->receiveOperationsFromServer(message);

}
void MessageProcesser::handleSystemMessage(std::shared_ptr<Message> message){
    
    std::shared_ptr<SystemMessage> systemMessage = std::dynamic_pointer_cast<SystemMessage>(message);
    if (!systemMessage){
        qWarning() << "SystemMessage is nullptr";
        return;
    }
    SystemCommandType systemCommandType = static_cast<SystemCommandType>(systemMessage->systemCommandType);
    switch(systemCommandType){  
        case SystemCommandType::SetUserUUID:
            {
                User::getInstance()->setUUID(systemMessage->content);
                break;
            }
        case SystemCommandType::SetUserID:
            {
                if (systemMessage->uuid == User::getInstance()->getUUID()){
                    User::getInstance()->setUserID(systemMessage->content);
                }
                break;
            }
        case SystemCommandType::SetUserTeam:
            {
                if (systemMessage->uuid == User::getInstance()->getUUID()){
                    User::getInstance()->setTeamType(static_cast<TeamType>(systemMessage->content.toInt()));
                }
                break;
            }
        case SystemCommandType::SetUserRole:
            {
                if (systemMessage->uuid == User::getInstance()->getUUID()){
                    // User::getInstance()->setRoleType(static_cast<RoleType>(systemMessage->content.toInt()));
                }
                break;
            }
        case SystemCommandType::CreateGame:
            {
                std::shared_ptr<Message> gameMessage = GameMessageFactory::createCreateGameMessage();
                emit m_networkDataManager->receiveOperationsFromServer(gameMessage);
                break;
            }
        case SystemCommandType::SetPlayerToTeam:
            {
                std::shared_ptr<Message> gameMessage = GameMessageFactory::createSetPlayerToTeamMessage(
                    systemMessage->uuid, static_cast<TeamType>(systemMessage->content.toInt())
                    ); 
                emit m_networkDataManager->receiveOperationsFromServer(gameMessage);
                break;
            }
        case SystemCommandType::AddPlayer:
            {
                std::shared_ptr<Message> gameMessage = GameMessageFactory::createAddPlayerMessage(
                    Player(systemMessage->uuid, systemMessage->userID)
                    );
                emit m_networkDataManager->receiveOperationsFromServer(gameMessage);
                break;
            }
        default:
            qWarning() << "Unknown SystemCommandType:" << static_cast<int>(systemCommandType);
            break;  
    }
 
 
}




