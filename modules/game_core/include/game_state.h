#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include "card_info.h"
enum class TeamType{
    Blue,
    Red,
    Unknown,
    God
};
class Player{
    public:
    QString uuid;
    QString name;
    TeamType teamType;
    Player(QString uuid, QString name): uuid(uuid), name(name){}
    QJsonObject toJson() const {
        QJsonObject jsonObj;
        jsonObj["uuid"] = uuid;
        jsonObj["name"] = name;
        jsonObj["teamType"] = static_cast<int>(teamType);
        return jsonObj;
    }
    static Player fromQJsonObject(const QJsonObject& jsonObj){
        Player player(jsonObj["uuid"].toString(), jsonObj["name"].toString());
        player.teamType = static_cast<TeamType>(jsonObj["teamType"].toInt());
        return player;
    }
};
class Team{
public:
    TeamType teamType;
    std::vector<std::shared_ptr<Player>> players;
    int score;
    bool isTurn;
    Team(TeamType teamType): teamType(teamType), score(score = 0), isTurn(false){}
    void addPlayer(std::shared_ptr<Player> player){
        player->teamType = teamType;
        players.push_back(player);
    }
    QJsonObject toJson() const {
        QJsonObject jsonObj;
        jsonObj["teamType"] = static_cast<int>(teamType);
        jsonObj["score"] = score;
        jsonObj["isTurn"] = isTurn;
        QJsonArray playersJsonArray;
        for (const auto& player : players) {
            playersJsonArray.append(player->toJson());
        }
        jsonObj["players"] = playersJsonArray;
        return jsonObj;
    }
    static Team fromQJsonObject(const QJsonObject& jsonObj){
        Team team(static_cast<TeamType>(jsonObj["teamType"].toInt()));
        team.score = jsonObj["score"].toInt();
        team.isTurn = jsonObj["isTurn"].toBool();
        QJsonArray playersJsonArray = jsonObj["players"].toArray();
        for (const auto& playerJson : playersJsonArray) {
            team.players.push_back(std::make_shared<Player>(Player::fromQJsonObject(playerJson.toObject())));
        }
        return team;
    }
};
class GameState{
private:
    friend class GameCore;

public:
    std::vector<CardInfo*> m_cardsInfo;
    Team blueTeam;
    Team redTeam;
    std::unordered_map<QString, std::shared_ptr<Player>> players;
    GameState(); 
    QJsonObject toJson() const;
    QJsonArray playersToJson() const;   
    QJsonArray cardsInfoToJson() const; 
    bool addPlayer(const std::shared_ptr<Player> player);
    void removePlayer(QString uuid);
    bool setPlayerToTeam(QString uuid, TeamType teamType);
    static GameState fromQJsonObject(const QJsonObject& jsonObj);
};



#endif // GAME_STATE_H  