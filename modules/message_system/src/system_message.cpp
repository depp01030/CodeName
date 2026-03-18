#include "system_message.h"

// ChatMessage definitions
SystemMessage::SystemMessage(SystemCommandType systemCommandType, const QString& content)
    : Message(MessageType::SystemMessage),
    systemCommandType(systemCommandType),
    content(content) {

    }

QJsonObject SystemMessage::toJson() const {
    QJsonObject jsonObj = Message::toJson();
    jsonObj["content"] = content; 
    jsonObj["systemCommandType"] = static_cast<int>(systemCommandType);
    return jsonObj;
}

QByteArray SystemMessage::toQByteArray() const {
    QJsonDocument doc(toJson());
    return doc.toJson();
}

std::shared_ptr<SystemMessage> SystemMessage::fromQByteArray(const QByteArray& byteArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObj = jsonDoc.object();
    return fromQJsonObject(jsonObj);
}

std::shared_ptr<SystemMessage> SystemMessage::fromQJsonObject(const QJsonObject& jsonObj) {
    QString content = jsonObj["content"].toString();
    SystemCommandType systemCommandType = static_cast<SystemCommandType>(jsonObj["systemCommandType"].toInt());
    std::shared_ptr<SystemMessage> systemMessage = std::make_shared<SystemMessage>(systemCommandType, content);
    systemMessage->userID = jsonObj["userID"].toString();
    systemMessage->userIP = jsonObj["userIP"].toString();
    systemMessage->uuid = jsonObj["uuid"].toString();
    systemMessage->timestamp = jsonObj["timestamp"].toVariant().toDateTime();
     
    return systemMessage;
}

bool SystemMessageFactory::isCommand(const QString& content){

    int index = content.indexOf('@'); 
    if (index == -1) return false;
    return true;
}

std::tuple<QString, SystemCommandType> SystemMessageFactory::getCommand(const QString& content){ 
    QString commandMessage = "";
    SystemCommandType command = SystemCommandType::Unknown;
    try{ 
        int index = content.indexOf('@'); 
        if (index == -1) return std::make_tuple(commandMessage, command);
        
        QString command_text = content.mid(index + 1).trimmed();
        int firstSpaceIndex = command_text.indexOf(' ');
        if (firstSpaceIndex != -1){
            commandMessage = command_text.mid(firstSpaceIndex + 1).trimmed();
        }
        command_text = command_text.left(firstSpaceIndex);
        
        // command_text = command_text.toLower();
        auto it = COMMANDS_MAP.find(command_text);
        if (it != COMMANDS_MAP.end()){
            command = it->second;
        }else{
            command = SystemCommandType::Unknown;
            qWarning() << "Unknown SystemCommandType:" << command_text;
        }
        return std::make_tuple(commandMessage, command);
        }
    catch (const std::exception& e) {
        qDebug() << e.what();
        return std::make_tuple(commandMessage, command);
    }
}
std::shared_ptr<SystemMessage> SystemMessageFactory::createSystemMessage(const QString& content){
    auto [command_text, commandType] = getCommand(content);
    return std::make_shared<SystemMessage>(commandType, command_text);
}
const std::unordered_map<QString, SystemCommandType> SystemMessageFactory::COMMANDS_MAP = {
    {"setTeam", SystemCommandType::SetPlayerToTeam},
    {"addPlayer", SystemCommandType::AddPlayer},
    {"setUUID", SystemCommandType::SetUserUUID},
    {"login", SystemCommandType::UserLogin},
    {"logout", SystemCommandType::UserLogout},
    {"setUserID", SystemCommandType::SetUserID},
    {"setUserTeam", SystemCommandType::SetUserTeam},
    {"setUserRole", SystemCommandType::SetUserRole},
    {"start", SystemCommandType::CreateGame},
    {"init", SystemCommandType::InitGame},
    {"help", SystemCommandType::Help},
};