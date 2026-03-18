#ifndef GAME_OPERATION_H
#define GAME_OPERATION_H

#include <QJsonDocument>
#include <QJsonObject>
#include <memory>
#include <variant>
#include "card_info.h"
 

enum class Operation{
    CreateGame,
    InitializeGame,
    ClickCard,
    UpdateScore,
    SwitchTeam,
    OpenCard,
    AddPlayer,
    SetPlayerToTeam,
};
class GameOperation{
    public:
    Operation operation;
    virtual ~GameOperation() = default;
    virtual QJsonObject toJson() const = 0;
    QByteArray toByteArray() const{
        QJsonDocument jsonDoc(toJson());
        return jsonDoc.toJson();
    }
    static std::unique_ptr<GameOperation> fromQByteArray(const QByteArray& byteArray);
    static std::unique_ptr<GameOperation> fromQJsonObject(const QJsonObject& jsonObj);
}; 
class OperationCreateGame : public GameOperation{
public:
    OperationCreateGame(){
        operation = Operation::CreateGame;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        return jsonObj;
    }
    static std::unique_ptr<OperationCreateGame> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationCreateGame>();
    }
};
class OperationInitializeGame : public GameOperation{
public:
    GameState* m_gameState;
    OperationInitializeGame(GameState* gameState):m_gameState(gameState){
        operation = Operation::InitializeGame;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["gameState"] = m_gameState->toJson();
        return jsonObj;
    }
    static std::unique_ptr<OperationInitializeGame> fromQJsonObject(const QJsonObject& jsonObj){
        std::unique_ptr<GameState> gameState = std::make_unique<GameState>(GameState::fromQJsonObject(jsonObj["gameState"].toObject()   ));
        // GameState gameState = GameState::fromQJsonObject(jsonObj["gameState"].toObject());  
        return std::make_unique<OperationInitializeGame>(gameState.release());
    }
};
class OperationClickCard : public GameOperation{
public:
    int cardID;
    CardState cardState;

    OperationClickCard(int cardID, CardState cardState): 
     cardID(cardID), cardState(cardState){
        operation = Operation::ClickCard;
     }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["cardID"] = cardID;
        jsonObj["cardState"] = static_cast<int>(cardState);
        return jsonObj;
    }
    static std::unique_ptr<OperationClickCard> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationClickCard>(jsonObj["cardID"].toInt(), static_cast<CardState>(jsonObj["cardState"].toInt()));
    }
};
class OperationOpenCard : public GameOperation{
public:
    int cardID;
    CardState cardState;

    OperationOpenCard(int cardID, CardState cardState): 
     cardID(cardID), cardState(cardState){
        operation = Operation::OpenCard;
     }
    QJsonObject toJson() const override{
        QJsonObject jsonObj; 
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["cardID"] = cardID;
        jsonObj["cardState"] = static_cast<int>(cardState);
        return jsonObj;
    }
    static std::unique_ptr<OperationOpenCard> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationOpenCard>(jsonObj["cardID"].toInt(), static_cast<CardState>(jsonObj["cardState"].toInt()));
    }
};
class OperationUpdateScore : public GameOperation{
public:
    TeamType team;
    int score;

    OperationUpdateScore(TeamType team, int score):team(team), score(score){
        operation = Operation::UpdateScore;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["team"] = static_cast<int>(team);
        jsonObj["score"] = score;
        return jsonObj;
    }
    static std::unique_ptr<OperationUpdateScore> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationUpdateScore>(static_cast<TeamType>(jsonObj["team"].toInt()), jsonObj["score"].toInt());
    }
};
class OperationSwitchTeam : public GameOperation{
public:
    TeamType team;

    OperationSwitchTeam(TeamType team):team(team){
        operation = Operation::SwitchTeam;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["team"] = static_cast<int>(team);
        return jsonObj;
    }
    static std::unique_ptr<OperationSwitchTeam> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationSwitchTeam>(static_cast<TeamType>(jsonObj["team"].toInt()));
    }
};
class OperationAddPlayer : public GameOperation{
public:
    Player player;
    OperationAddPlayer(const Player& player):player(player){
        operation = Operation::AddPlayer;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["player"] = player.toJson();
        return jsonObj;
    }
    static std::unique_ptr<OperationAddPlayer> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationAddPlayer>(Player::fromQJsonObject(jsonObj["player"].toObject()));
    }
};      
class OperationSetPlayerToTeam : public GameOperation{
public:
    QString uuid;
    TeamType teamType;
    OperationSetPlayerToTeam(const QString& uuid, TeamType teamType):uuid(uuid), teamType(teamType){
        operation = Operation::SetPlayerToTeam;
    }
    QJsonObject toJson() const override{
        QJsonObject jsonObj;
        jsonObj["operation"] = static_cast<int>(operation);
        jsonObj["uuid"] = uuid;
        jsonObj["teamType"] = static_cast<int>(teamType);
        return jsonObj;
    }
    static std::unique_ptr<OperationSetPlayerToTeam> fromQJsonObject(const QJsonObject& jsonObj){
        return std::make_unique<OperationSetPlayerToTeam>(jsonObj["uuid"].toString(), static_cast<TeamType>(jsonObj["teamType"].toInt()));
    }
};      

#endif // GAME_OPERATION_H

