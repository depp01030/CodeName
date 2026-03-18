#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <iostream>
#include <vector>
#include <QColor>
#include <QObject>
#include <QDebug>
#include "candidate_words.h"
#include "card_color.h"
#include "game_state.h"
#include "card_info.h"


class GameConfig{
    public:
    friend class GameCore;
    const int m_rowNum = 5;
    const int m_colNum = 5;
    size_t m_totalCardNum = m_rowNum * m_colNum;
    size_t m_spyCardNum = 8; 
    size_t m_killCardNum = 2;
};
class GameCore : public QObject {
    Q_OBJECT  // 添加這行
public:  
private: 
    friend class GameController; 
    GameConfig m_gameConfig;
    GameState *m_gameState;
public: 
    GameCore(QObject *parent = nullptr);
    ~GameCore(){};
    GameState* createGame();
    void setGameState(GameState* gameState);
    std::vector<CardInfo*> getCardsInfo() const;
    void addPlayer(const std::shared_ptr<Player> player);
    void setPlayerToTeam(QString uuid, TeamType teamType);
    int getTeamScore(TeamType teamType) const;
    void openCard(CardInfo& cardInfo);
    Team& getTeam(TeamType teamType) { return teamType == TeamType::Blue ? m_gameState->blueTeam : m_gameState->redTeam; }
    Team& getCurrentTeam() { return m_gameState->blueTeam.isTurn ? m_gameState->blueTeam : m_gameState->redTeam; } 
    Team& getNextTeam()  { return m_gameState->blueTeam.isTurn ? m_gameState->redTeam : m_gameState->blueTeam; } 
    void setCurrentTeam(TeamType teamType);


    bool checkIsCardAnswer(const CardInfo& cardInfo) const;
    void updateTeamScore(TeamType teamType, int score);
    void setTeamScore(TeamType teamType, int score);
    void addTeamScore(TeamType teamType, int score);
private:
    void generateCardsInfo(); 
    void setCardsInfo(std::vector<CardInfo*> cardsInfo); 
    std::vector<CardInfo*> randomGenerateCardInfos();
signals:
    void updateGameInfo();
    void cardOpened(const CardInfo& cardInfo);
    void teamSwitched(const TeamType teamType);
    void teamScoreUpdated(const TeamType teamType, const int score);
    void playerAdded(const Player& player);
    void playerSetToTeam(const QString& uuid, const TeamType teamType);

};

#endif // GAME_CORE_H
















