#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

#include <iostream>
#include <memory>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include "message.h"
#include "game_state.h"
#include "game_operation.h"

class GameMessage : public Message {
public:
    std::unique_ptr<GameOperation> operationData;

    // Constructor
    GameMessage(std::unique_ptr<GameOperation> operationData)
        : Message(MessageType::GameMessage), operationData(std::move(operationData)) {}

    // Convert to JSON
    QJsonObject toJson() const override;
    // Convert to QByteArray
    QByteArray toQByteArray() const override;
    // Static method to construct GameMessage from QByteArray
    static std::shared_ptr<GameMessage> fromQByteArray(const QByteArray& byteArray);
    static std::shared_ptr<GameMessage> fromQJsonObject(const QJsonObject& jsonObj);
};

class GameMessageFactory {
public:
    static std::shared_ptr<GameMessage> createCreateGameMessage() {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationCreateGame>()
        );
    }
    static std::shared_ptr<GameMessage> createInitializeGameMessage(GameState* gameState) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationInitializeGame>(gameState)
        );
    }
    static std::shared_ptr<GameMessage> createClickCardMessage(int cardID, CardState state) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationClickCard>(cardID, state)
        );
    }
    static std::shared_ptr<GameMessage> createOpenCardMessage(int cardID, CardState state) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationOpenCard>(cardID, state)
        );
    }

    static std::shared_ptr<GameMessage> createUpdateScoreMessage(TeamType team, int score) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationUpdateScore>(team, score)
        );
    }

    static std::shared_ptr<GameMessage> createSwitchTeamMessage(TeamType team) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationSwitchTeam>(team)
        );
    }
    static std::shared_ptr<GameMessage> createAddPlayerMessage(const Player& player) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationAddPlayer>(player)
        );
    }
    static std::shared_ptr<GameMessage> createSetPlayerToTeamMessage(const QString& uuid, TeamType teamType) {
        return std::make_shared<GameMessage>(
            std::make_unique<OperationSetPlayerToTeam>(uuid, teamType)
        );
    }
};

#endif // GAME_MESSAGE_H
