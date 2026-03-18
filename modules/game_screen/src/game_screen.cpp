#include <QPainter>
#include "ui_game_screen.h"
#include "game_screen.h"
#include "game_state.h"
#include "user.h"
#include <QGraphicsDropShadowEffect>

GameScreen::GameScreen(QWidget *parent, GameController *gameController):
    QWidget(parent),
    ui(new Ui::GameScreen),
    m_gameController(gameController)
{
    setupUI();  // 添加這行來調用setupUI()方法
}

GameScreen::~GameScreen()
{
    delete ui;
    delete m_gameController;

}


void GameScreen::setupUI() {   
    ui->setupUi(this);
    this->ui->label_redScore->setText(QString("探員分數 0"));
    this->ui->label_blueScore->setText(QString("刺客分數 0"));

    /* General Background and Text with Cyberpunk Borders */
    QString boardStyleSheet = 
        "QWidget { "
        "background-color: #121212; /* Dark background */"
        "color: #E0E0E0; /* Light gray text */"
        "border: 2px solid #00FFFF; /* Neon cyan border */" 
        "padding: 2px; /* Padding inside the border */" 
        "}";
    this->ui->ScoreBoadWidget->setStyleSheet( boardStyleSheet );
    this->ui->redTeamWidget->setStyleSheet( boardStyleSheet );
    this->ui->blueTeamWidget->setStyleSheet( boardStyleSheet );
    setupBlurEffect();
}
void GameScreen::setupBlurEffect() {
    // QGraphicsDropShadowEffect *m_shadowEffect = new QGraphicsDropShadowEffect(this);
    // m_shadowEffect->setColor(Qt::yellow);
    // m_shadowEffect->setBlurRadius(30);
    // m_shadowEffect->setOffset(0, 0);
 
    // this->setGraphicsEffect(m_shadowEffect);
}
void GameScreen::setupCodeCards(const std::vector<CardInfo*> cardsInfo) {  
    qDebug() << "GameScreen setupCodeCards";

    for (const auto& cardInfo : cardsInfo){
        CodeCard *card = new CodeCard(
            cardInfo->m_id,
            cardInfo->m_word,
            this
        ); 
        size_t row = cardInfo->m_index / m_gameController->getRowNum();    
        size_t col = cardInfo->m_index % m_gameController->getColNum();
        ui->gridLayout_cards->addWidget(card, row, col);
        m_codeCards[cardInfo->m_id] = card;
        if (User::getInstance()->getTeamType() == TeamType::Blue || User::getInstance()->getTeamType() == TeamType::God ){
            card->setCommanderStyle(cardInfo->m_answerColor);
        }


    }
}
void GameScreen::setCurrentTeam(const TeamType team) {
    if (team == TeamType::Blue){
        this->ui->label_round->setStyleSheet("background-color: lightblue;");
        this->ui->label_round->setText("藍方回合");
        // this->ui->label_blueTeam->setStyleSheet("background-color: lightblue;");
    }
    else{
        this->ui->label_round->setStyleSheet("background-color: lightpink;");
        this->ui->label_round->setText("紅方回合");
        // this->ui->label_redTeam->setStyleSheet("background-color: lightpink;");
    }
}
void GameScreen::addPlayer(const Player& player) {
    
    QLabel *playerLabel = this->findChild<QLabel*>(player.uuid);
 
    playerLabel = new QLabel(player.name);
    playerLabel->setObjectName(player.uuid);
    if (player.teamType == TeamType::Blue){
        this->ui->blueTeamPlayersLayout->addWidget(playerLabel);
    }
    else{
        this->ui->redTeamPlayersLayout->addWidget(playerLabel);
    }
}
void GameScreen::setPlayerToTeam(const QString& uuid, const TeamType teamType) {
    qDebug() << "GameScreen::setPlayerToTeam" << uuid << static_cast<int>(teamType);
    QLabel *playerLabel = this->findChild<QLabel*>(uuid);
    if (playerLabel) {
        if (teamType == TeamType::Blue){
            if (this->ui->redTeamPlayersLayout->indexOf(playerLabel) != -1){
                this->ui->redTeamPlayersLayout->removeWidget(playerLabel);
            }
            if (this->ui->blueTeamPlayersLayout->indexOf(playerLabel) == -1){
                this->ui->blueTeamPlayersLayout->addWidget(playerLabel);    
            }
        } else if (teamType == TeamType::Red){
            if (this->ui->blueTeamPlayersLayout->indexOf(playerLabel) != -1){   
                this->ui->blueTeamPlayersLayout->removeWidget(playerLabel); 
            }
            if (this->ui->redTeamPlayersLayout->indexOf(playerLabel) == -1){
                this->ui->redTeamPlayersLayout->addWidget(playerLabel);
            }
        }
    } else {
        qWarning() << "Player not found";
    }
}
void GameScreen::setTeamScore(const TeamType team, const int score) {
    // QString round_msg = QString("%1隊分數 %2");
    if (TeamType::Red == team){
        this->ui->label_redScore->setText(QString("探員分數 %1").arg(score) );
    }
    else if (TeamType::Blue == team){
        this->ui->label_blueScore->setText(QString("刺客分數 %1").arg(score) );
    }

    
}
void GameScreen::setupConnections() { 
    // set up the connection between the card and the game screen
    for (auto &pair : m_codeCards){ 
        disconnect(pair.second, &CodeCard::clicked, this, nullptr); // Disconnect any existing connections
        connect(pair.second, &CodeCard::clicked, this, [this, pair](){
            this->onCodeCardClicked(pair.second);
        }); 
    }
}
void GameScreen::onCodeCardClicked(const CodeCard* codeCard) {
    // qDebug() << "Card clicked:" << codeCard->m_word;
    m_gameController->onCodeCardClicked(codeCard->m_id);   

}

void GameScreen::openCard(const CardInfo& cardInfo) {
    if (m_codeCards.find(cardInfo.m_id) == m_codeCards.end()){
        qDebug() << "Card not found";
        return;
    }
    CodeCard *card = m_codeCards[cardInfo.m_id];
    card->turnOpen(cardInfo.m_answerColor);
    // qDebug() << "Card answer:" << std::to_string(cardInfo.m_isAnswer);
}



