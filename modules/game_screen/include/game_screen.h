#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <vector>
#include <unordered_map>
// #include "network_data_manager.h"
#include "game_state.h"
#include "game_controller.h" 

#include "code_card.h" 

namespace Ui {
    class GameScreen;
} 
class GameController; // 前向聲明
class Player;
class GameScreen : public QWidget {
    Q_OBJECT

public: 
    explicit GameScreen(QWidget *parent = nullptr, GameController *gameController = nullptr);
    ~GameScreen();

private: 
    friend class GameController;
    
    std::unordered_map<int, CodeCard*> m_codeCards; //id, card
    GameController *m_gameController;
    Ui::GameScreen *ui;
    void setupUI();
    void setupCodeCards(const std::vector<CardInfo*> cardsInfo);
    void setCurrentTeam(const TeamType team);
    void setTeamScore(const TeamType team, const int score);
    void openCard(const CardInfo& cardInfo);
    void setupConnections(); 
private slots:
    void setupBlurEffect();
    void addPlayer(const Player& player);
    void setPlayerToTeam(const QString& uuid, const TeamType teamType);
    void onCodeCardClicked(const CodeCard* codeCard); 
};

#endif // GAME_SCREEN_H
