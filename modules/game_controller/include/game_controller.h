#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include <iostream>
#include <vector>
#include <QColor>
#include <QObject>
#include <QDebug> 
#include <memory> 

class Player;
class Message;
class GameState;
enum class TeamType;
class GameScreen;
class GameCore;
class NetworkDataManager;

class GameController : public QObject {
    Q_OBJECT  // 添加這行

public: 
    GameController(QObject *parent = nullptr, NetworkDataManager *server = nullptr);
    ~GameController();
    int getRowNum() const;
    int getColNum() const;
    
private: 
    
signals:
    void sendUserGameOperationToServer(std::shared_ptr<Message> gameMessage);
    void broadcastServerGameOperation(std::shared_ptr<Message> gameMessage);
private:
    friend class MainWindow;
    friend class GameScreen;
    GameScreen *m_gameScreen;
    GameCore *m_gameCore;
    NetworkDataManager *m_server;
    void setupConnections();
    // Message* packMessage(const OperationType operation, const CardInfo *cardInfo);
    // Message* packMessage(const OperationType operation, const TeamType &teamType);
    // Message* packMessage(const OperationType operation, const TeamType &teamType, const int &score);   
    
private slots:
    void createGame();
    void doAddPlayer(const Player& player);
    void doSetPlayerToTeam(const QString& uuid, const TeamType teamType);
    void initGame(GameState* gameState); 
    void userClickCard(const int cardID);
    void doOpenCard(const int cardID);
    void doTeamSwitch(TeamType team);
    void doUpdateScore(TeamType team, int score);
    void onCodeCardClicked(const int id);
    void handleServerOperations(std::shared_ptr<Message> gameMessage);
    void joinTeam(TeamType team); 
    void startGame();
    // void onCardOpened(const CardInfo* cardInfo);
    // void onTeamScoreUpdated(const TeamType &teamType, const int score);
};

#endif // GAME_CORE_H
  
