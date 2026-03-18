#include <algorithm>
#include <random>
#include <functional>
#include "user.h"
#include "game_core.h"
GameCore::GameCore(QObject *parent) : 
    QObject(parent), m_gameConfig(GameConfig()), m_gameState(new GameState()){
}
GameState* GameCore::createGame() {
    GameState* gameState = new GameState();
    // setCardsInfo(gameState);  
    return gameState;
}
void GameCore::generateCardsInfo(){
    m_gameState->m_cardsInfo = randomGenerateCardInfos();
}
std::vector<CardInfo*> GameCore::getCardsInfo() const {
    return m_gameState->m_cardsInfo;
}
void GameCore::setCardsInfo(std::vector<CardInfo*> cardsInfo)
{
    m_gameState->m_cardsInfo = cardsInfo;
}
void GameCore::addPlayer(const std::shared_ptr<Player> player){
    bool isAdded = m_gameState->addPlayer(player);
    if (isAdded){ 
        if (player->uuid == User::getInstance()->getUUID()){ 
            User::getInstance()->setTeamType(player->teamType);
        }
        emit playerAdded(*player);
    }
}
void GameCore::setPlayerToTeam(QString uuid, TeamType teamType){
    bool isSet = m_gameState->setPlayerToTeam(uuid, teamType);
    if (isSet){
        if (uuid == User::getInstance()->getUUID()){
            User::getInstance()->setTeamType(teamType);
        }
        emit playerSetToTeam(uuid, teamType);
    }
}

void GameCore::setGameState(GameState* gameState) { 
    m_gameState = gameState;
 
    setCurrentTeam(gameState->blueTeam.isTurn ? TeamType::Blue : TeamType::Red);
    setTeamScore(TeamType::Blue, gameState->blueTeam.score);
    setTeamScore(TeamType::Red, gameState->redTeam.score);
    // m_gameState.blueTeam.isTurn = true; 
}
std::vector<CardInfo*> GameCore::randomGenerateCardInfos() {
    std::vector<size_t> indices(CANDIDATE_WORDS.size()); //生成一個vector indices with length CANDIDATE_WORDS.size()
    std::iota(indices.begin(), indices.end(), 0); //將indices填入0到CANDIDATE_WORDS.size()的數字
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g); //將indices的數字打亂
    std::vector<CardInfo*> cardsInfo;
    
    for (size_t i = 0; i < m_gameConfig.m_totalCardNum; ++i) {
        if (i < m_gameConfig.m_spyCardNum){
            cardsInfo.emplace_back(new CardInfo(CANDIDATE_WORDS[indices[i]], CardType::GreenSpy, true));
        }
        else if (i < m_gameConfig.m_spyCardNum + m_gameConfig.m_killCardNum){
            cardsInfo.emplace_back(new CardInfo(CANDIDATE_WORDS[indices[i]], CardType::Mine, false));
        }
        else{
            cardsInfo.emplace_back(new CardInfo(CANDIDATE_WORDS[indices[i]], CardType::People, false));
        }
    }
    std::shuffle(cardsInfo.begin(), cardsInfo.end(), g);
    size_t index = 0;
    for (CardInfo* cardInfo : cardsInfo){
        cardInfo->m_id = index;
        cardInfo->m_index = index;
        index ++;
    }
    return cardsInfo;
} 



void GameCore::openCard(CardInfo& cardInfo) {
    if (cardInfo.m_state == CardState::Open) {
        qDebug() << "GameCore : Card already open";
        return;
    }
    cardInfo.m_state = CardState::Open;
    emit cardOpened(cardInfo);
}
 
    
void GameCore::setCurrentTeam(TeamType teamType){
    m_gameState->blueTeam.isTurn = teamType == TeamType::Blue;
    m_gameState->redTeam.isTurn = teamType == TeamType::Red;
    emit teamSwitched(teamType); 
}
int GameCore::getTeamScore(TeamType teamType) const {
    if (teamType == TeamType::Blue) {
        return m_gameState->blueTeam.score;
    }
    else if (teamType == TeamType::Red){    
        return m_gameState->redTeam.score;
    }
    else{
        qDebug() << "Invalid team";
        return 0;
    }
}
void GameCore::addTeamScore(TeamType teamType, int score){
    Team& team = getTeam(teamType);
    team.score += score;
}
void GameCore::setTeamScore(TeamType teamType, int score){
    Team& team = getTeam(teamType);
    team.score = score;
    emit teamScoreUpdated(teamType, score);
}

    
bool GameCore::checkIsCardAnswer(const CardInfo& cardInfo) const {
    if (cardInfo.m_isAnswer) {
        return true;
    }
    return false;
}
