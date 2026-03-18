#include <algorithm>
#include <random>
#include <functional>

#include "network_data_manager.h"
#include "message.h"
#include "game_state.h"
#include "game_core.h"
#include "game_screen.h" 
#include "game_message.h"
#include "user.h"
#include "game_controller.h"
#include "chat_message.h"
#include "ui_game_screen.h"
#include "system_message.h"
// #include <QSignalSpy>
 
GameController::GameController(QObject *parent, NetworkDataManager *server): 
QObject(parent),
m_server(server), m_gameCore(new GameCore(this)),
m_gameScreen(new GameScreen(nullptr, this))
{ 
    // initGame();
    setupConnections();
    // if (User::getInstance()->getUserID() != "Server"){
    //     std::shared_ptr<Message> systemMessage = SystemMessageFactory::createSystemMessage(QString("@addPlayer"));
    //     emit sendUserGameOperationToServer(systemMessage);
    // }
}

GameController::~GameController() {
    delete m_gameCore;
    delete m_gameScreen;
} 
int GameController::getRowNum() const { return m_gameCore->m_gameConfig.m_rowNum; }
int GameController::getColNum() const { return m_gameCore->m_gameConfig.m_colNum; }
void GameController::createGame() {
    // SERVER ONLY 

    m_gameCore->generateCardsInfo();
    initGame(m_gameCore->m_gameState);
    std::shared_ptr<Message> gameMessage = GameMessageFactory::createInitializeGameMessage(m_gameCore->m_gameState);
    emit broadcastServerGameOperation(gameMessage);
}
void GameController::initGame(GameState* gameState) {  
    // m_gameCore->setGameState(gameState);  
    {
        std::cout << "Game Init" << std::endl;
      
        doUpdateScore(TeamType::Blue, 0);
        doUpdateScore(TeamType::Red, 0);

        std::shared_ptr<Message> gameMessage = GameMessageFactory::createUpdateScoreMessage(TeamType::Blue, 0);
        emit broadcastServerGameOperation(gameMessage);
        gameMessage = GameMessageFactory::createUpdateScoreMessage(TeamType::Red, 0);
        emit broadcastServerGameOperation(gameMessage);
    }
    m_gameCore->setCardsInfo(gameState->m_cardsInfo);
    m_gameScreen->setupCodeCards(
        m_gameCore->getCardsInfo()
    ); 
    m_gameScreen->setupConnections(); //card clicked connection 
} 

void GameController::setupConnections() { 
    
    //connect to server
    // disconnect(this, &GameController::sendGameMessageToServer, m_server, nullptr);
    connect(this, &GameController::sendUserGameOperationToServer, m_server, &NetworkDataManager::onReceivingLocalMessage);
    connect(this, &GameController::broadcastServerGameOperation, m_server, &NetworkDataManager::onBroadcastServerGameOperation);
 
    // disconnect(m_server, &NetworkDataManager::serverOperationOpenCard, this, nullptr); 
    connect(m_server, &NetworkDataManager::receiveOperationsFromServer, this, &GameController::handleServerOperations); 
    
    // connect to game screen 
    connect(m_gameScreen->ui->joinRedButton, &QPushButton::clicked, this, [this]() {
        this->joinTeam(TeamType::Red);
    });
    connect(m_gameScreen->ui->joinBlueButton, &QPushButton::clicked, this, [this]() {
        this->joinTeam(TeamType::Blue);
    });
    connect(m_gameScreen->ui->gameStartButton, &QPushButton::clicked, this, [this]() {
        this->startGame();
    });
    connect(m_gameCore, &GameCore::playerAdded, m_gameScreen, &GameScreen::addPlayer);
    connect(m_gameCore, &GameCore::playerSetToTeam, m_gameScreen, &GameScreen::setPlayerToTeam);
    //翻牌
    // disconnect(m_gameCore, &GameCore::cardOpened, m_gameScreen, nullptr); 
    connect(m_gameCore, &GameCore::cardOpened, m_gameScreen, &GameScreen::openCard); 
    //換回合
    // disconnect(m_gameCore, &GameCore::teamSwitched, m_gameScreen, nullptr);
    connect(m_gameCore, &GameCore::teamSwitched, m_gameScreen, &GameScreen::setCurrentTeam);
    //更新分數
    // disconnect(m_gameCore, &GameCore::teamScoreUpdated, m_gameScreen, nullptr); 
    connect(m_gameCore, &GameCore::teamScoreUpdated, m_gameScreen, &GameScreen::setTeamScore); 
}
 
void GameController::joinTeam(TeamType team) {

    if (m_gameCore->m_gameState->players.find(User::getInstance()->getUUID()) == m_gameCore->m_gameState->players.end()){
        std::shared_ptr<Message> systemMessage = SystemMessageFactory::createSystemMessage(QString("@addPlayer"));
        emit sendUserGameOperationToServer(systemMessage);
    }
    std::shared_ptr<Message> systemMessage = SystemMessageFactory::createSystemMessage(QString("@setTeam %1").arg(static_cast<int>(team)));
    emit sendUserGameOperationToServer(systemMessage);

}
void GameController::startGame() {
    std::shared_ptr<Message> systemMessage = SystemMessageFactory::createSystemMessage(QString("@start"));
    emit sendUserGameOperationToServer(systemMessage);
    // m_gameScreen->ui->gameStartButton->setVisible(false);
}
void GameController::onCodeCardClicked(const int id) {
    //get cardinfo by id
    if (User::getInstance()->getTeamType() != m_gameCore->getCurrentTeam().teamType){
        qDebug() << "GameController::onCodeCardClicked: Not your turn";
        return;
    }
    CardInfo *cardInfo = m_gameCore->getCardsInfo()[id];
    if (cardInfo->m_state == CardState::Open) {
        qDebug() << "GameContr : Card already open";
        return;
    }
    // pack message
    std::shared_ptr<Message> gameMessage = GameMessageFactory::createClickCardMessage(cardInfo->m_id, cardInfo->m_state);  
    emit sendUserGameOperationToServer(gameMessage);
}

void GameController::userClickCard(const int cardID){
    qDebug() << "GameController::userClickCard";
    //get cardinfo by id
    CardInfo *cardInfo = m_gameCore->getCardsInfo()[cardID];

    // operate m_gameCore
    doOpenCard(cardInfo->m_id);  
    std::shared_ptr<Message> gameMessage = GameMessageFactory::createOpenCardMessage(cardInfo->m_id, cardInfo->m_state);  
    emit broadcastServerGameOperation(gameMessage);

    if (!m_gameCore->checkIsCardAnswer(*cardInfo)){ //switch
        if (cardInfo->m_cardType == CardType::Mine){
            // you dead

            std::shared_ptr<Message> chatMessage = ChatMessageFactory::createChatMessage(
                QString("You are killed by Killer")
            );
            emit sendUserGameOperationToServer(chatMessage);
            return;

        } 
        //update 刺客分數
        TeamType team = TeamType::Blue;
        int score = m_gameCore->getTeamScore(team) + 1;
        doUpdateScore(team, score); 
        std::shared_ptr<Message> gameMessage = GameMessageFactory::createUpdateScoreMessage(team, score);
        emit broadcastServerGameOperation(gameMessage);
        // TeamType nextTeam = m_gameCore->getNextTeam().teamType;
        // doTeamSwitch(nextTeam); 
        // std::shared_ptr<Message> gameMessage = GameMessageFactory::createSwitchTeamMessage(nextTeam);
        // emit broadcastServerGameOperation(gameMessage);
    }  
    else{ // update score
        TeamType team = m_gameCore->getCurrentTeam().teamType;
        int score = m_gameCore->getTeamScore(team) + 1;
        doUpdateScore(team, score);

        std::shared_ptr<Message> gameMessage = GameMessageFactory::createUpdateScoreMessage(team, score);
        emit broadcastServerGameOperation(gameMessage);
    }  
}
void GameController::doOpenCard(const int cardID){
    //get cardinfo by id
    CardInfo *cardInfo = m_gameCore->getCardsInfo()[cardID];
    m_gameCore->openCard(*cardInfo);  
}

void GameController::doTeamSwitch(TeamType team){
    qDebug() << "GameController::doTeamSwitch" << static_cast<int>(team);
    m_gameCore->setCurrentTeam(team); 
}

void GameController::doUpdateScore(TeamType team, int score){
    
    std::cout << "update score: team " << static_cast<int>(team) << " score: " << score << std::endl;
    m_gameCore->setTeamScore(team, score);
    // qDebug() << "Update score: " << jsonObj;  
}

void GameController::doAddPlayer(const Player& player){
    qDebug() << "GameController::doAddPlayer";
    std::shared_ptr<Player> playerPtr = std::make_shared<Player>(player);
    m_gameCore->addPlayer(playerPtr);
}
void GameController::doSetPlayerToTeam(const QString& uuid, const TeamType teamType){
    m_gameCore->setPlayerToTeam(uuid, teamType);
}

void GameController::handleServerOperations(std::shared_ptr<Message> message){
    std::shared_ptr<GameMessage> gameMessage = std::dynamic_pointer_cast<GameMessage>(message); 
    Operation operationType = gameMessage->operationData->operation ;   
     
    switch (operationType) {
        case Operation::CreateGame:{
            OperationCreateGame* operationData = dynamic_cast<OperationCreateGame*>(gameMessage->operationData.get()); 
            createGame();
            break;
        }
        case Operation::InitializeGame: {
            OperationInitializeGame* operationData = dynamic_cast<OperationInitializeGame*>(gameMessage->operationData.get()); 
            initGame(operationData->m_gameState);
            break;
        }
        case Operation::ClickCard: {
            OperationClickCard* operationData = dynamic_cast<OperationClickCard*>(gameMessage->operationData.get()); 
            userClickCard(operationData->cardID);     
            break;
        }
        case Operation::OpenCard: {
            OperationOpenCard* operationData = dynamic_cast<OperationOpenCard*>(gameMessage->operationData.get()); 
            doOpenCard(operationData->cardID);     
            break;
        }
        case Operation::SwitchTeam: {
            OperationSwitchTeam* operationData = dynamic_cast<OperationSwitchTeam*>(gameMessage->operationData.get()); 
            doTeamSwitch(operationData->team);
            break;
        }
        case Operation::UpdateScore: {
            OperationUpdateScore* operationData = dynamic_cast<OperationUpdateScore*>(gameMessage->operationData.get()); 
            doUpdateScore(operationData->team, operationData->score);
            break;
        } 
        case Operation::AddPlayer: {
            OperationAddPlayer* operationData = dynamic_cast<OperationAddPlayer*>(gameMessage->operationData.get()); 
            doAddPlayer(operationData->player);
            break;
        }
        case Operation::SetPlayerToTeam: {
            OperationSetPlayerToTeam* operationData = dynamic_cast<OperationSetPlayerToTeam*>(gameMessage->operationData.get()); 
            doSetPlayerToTeam(operationData->uuid, operationData->teamType);
            break;
        }
        default:
            qWarning() << "Unknown Operation:" << static_cast<int>(operationType);
            break;
    }
} 
