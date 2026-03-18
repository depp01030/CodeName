#include "game_message.h"


QByteArray GameMessage::toQByteArray() const {
    QJsonDocument jsonDoc(toJson());
    return jsonDoc.toJson();
} 
QJsonObject GameMessage::toJson() const {
    QJsonObject jsonObj = Message::toJson();
    // jsonObj["operation"] = static_cast<int>(operationData->operation);  // Store operation as integer
    jsonObj["operationData"] = operationData->toJson();

    return jsonObj;
}

std::shared_ptr<GameMessage> GameMessage::fromQByteArray(const QByteArray& byteArray) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObj = jsonDoc.object();
    return fromQJsonObject(jsonObj);
}


std::shared_ptr<GameMessage> GameMessage::fromQJsonObject(const QJsonObject& jsonObj) {
    // Extract operation type
    Operation op = static_cast<Operation>(jsonObj["operationData"]["operation"].toInt());
    std::unique_ptr<GameOperation> operationData;
    QJsonObject operationJsonObj = jsonObj["operationData"].toObject();
    switch (op) {
        case Operation::CreateGame: {
            operationData = OperationCreateGame::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::InitializeGame: {
            operationData = OperationInitializeGame::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::ClickCard: { 
            operationData = OperationClickCard::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::OpenCard: { 
            operationData = OperationOpenCard::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::UpdateScore: { 
            operationData = OperationUpdateScore::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::SwitchTeam: { 
            operationData = OperationSwitchTeam::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::AddPlayer: { 
            operationData = OperationAddPlayer::fromQJsonObject(operationJsonObj);
            break;
        }
        case Operation::SetPlayerToTeam: { 
            operationData = OperationSetPlayerToTeam::fromQJsonObject(operationJsonObj);
            break;
        }
        default: {
            qWarning() << "Unknown Operation:" << static_cast<int>(op);
            break;
        }
    }
    std::shared_ptr<GameMessage> gameMessage = std::make_shared<GameMessage>(std::move(operationData));
    gameMessage->userID = jsonObj["userID"].toString();
    gameMessage->userIP = jsonObj["userIP"].toString();
    gameMessage->timestamp = jsonObj["timestamp"].toVariant().toDateTime();
    return gameMessage;
}

