#include "game_state.h"

GameState::GameState() : blueTeam(TeamType::Blue), redTeam(TeamType::Red) {

}
QJsonObject GameState::toJson() const{
    QJsonObject jsonObj;
    jsonObj["blueTeam"] = blueTeam.toJson();
    jsonObj["redTeam"] = redTeam.toJson();
    jsonObj["cardsInfo"] = cardsInfoToJson();
    jsonObj["players"] = playersToJson();
    return jsonObj;
}
GameState GameState::fromQJsonObject(const QJsonObject& jsonObj){
    GameState gameState;
    gameState.blueTeam = Team::fromQJsonObject(jsonObj["blueTeam"].toObject());
    gameState.redTeam = Team::fromQJsonObject(jsonObj["redTeam"].toObject());
    QJsonArray cardsInfoJsonArray = jsonObj["cardsInfo"].toArray();
    for (const auto& cardInfoJson : cardsInfoJsonArray) {
        CardInfo cardInfo = CardInfo::fromQJsonObject(cardInfoJson.toObject()); 
        gameState.m_cardsInfo.push_back(new CardInfo(std::move(cardInfo)));
    }
    qDebug()<< "cardsInfo size: " << gameState.m_cardsInfo.size();
    return gameState;
}
QJsonArray GameState::cardsInfoToJson() const {
    QJsonArray jsonArray;
    for (const auto& cardInfo : m_cardsInfo) {
        jsonArray.append(cardInfo->toJson());
    }
    return jsonArray;
}

QJsonArray GameState::playersToJson() const {
    QJsonArray jsonArray;
    for (const auto& player : players) {
        jsonArray.append(player.second->toJson());
    }
    return jsonArray;
}

void GameState::removePlayer(QString uuid){
    if (players.find(uuid) != players.end()){
        players.erase(uuid);
    }
}
bool GameState::addPlayer(const std::shared_ptr<Player> player){
    if (players.find(player->uuid) != players.end()){
        return false;
    }
    if (player->teamType == TeamType::Blue){     
        blueTeam.addPlayer(player);
    } else {
        redTeam.addPlayer(player);
    } 
    players[player->uuid] = player;
    qDebug() << "player uuid: " << player->uuid;
    qDebug() << "players size: " << players.size();
    return true;
}

bool GameState::setPlayerToTeam(QString uuid, TeamType teamType){
    if (players.find(uuid) == players.end()){
        qDebug() << "Player not found";
        return false;
    }
    if (teamType == TeamType::Blue){
        blueTeam.addPlayer(players[uuid]);
    } else if (teamType == TeamType::Red){
        redTeam.addPlayer(players[uuid]);
    }
    else{
        qDebug()<< "Invalid team type";
    }
    return true;
}

