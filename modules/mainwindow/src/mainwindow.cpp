#include <QFile>
#include <QTextStream>
#include <QString>
#include "network_data_factory.h"
#include "chatroom.h"
#include "game_controller.h"
#include "game_screen.h"
#include "user.h"
#include "game_state.h"
#include "mainwindow.h" 
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_networkDataManager(NetworkDataManagerFactory::createNetworkDataManager(this))
{
    #ifdef BUILD_SERVER 
        User::setInstance("Server", m_networkDataManager->getIPAddress());
        User::getInstance()->setTeamType(TeamType::God); 
        this->isServer = true;
    #else
        User::setInstance(m_networkDataManager->getIPAddress());
    #endif
    setupConnection(); 
    setupUi();

}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_networkDataManager;
    delete m_chatRoom;  
    delete m_gameController;    
}
void MainWindow::setupUi()
{
    
    ui->setupUi(this);
    setupUserInfo();
    m_chatRoom = new ChatRoom(this, m_networkDataManager);
    setupWidgets(ui->chatRoomPlaceholder,
     m_chatRoom
    );

    m_gameController = new GameController(this, m_networkDataManager);
    setupWidgets(ui->gameScreenPlaceholder,
     m_gameController->m_gameScreen
    );
    if (this->isServer) {
        this->setWindowTitle("ChatRoom Server");
    }
    // Apply dark theme
    loadTheme(); 
}
void MainWindow::setupUserInfo(){
    ui->labelUserIDValue->setText(
        User::getInstance()->getUserID()
    );
    // ui->labelTeamValue->setText(
    //     (User::getInstance()->getStrTeamType())
    // );

}
void MainWindow::setupConnection()
{
    connect(User::getInstance(), &User::uuidChanged, this, &MainWindow::updateUUID);
    connect(User::getInstance(), &User::userIDChanged, this, &MainWindow::updateUserID);
    connect(User::getInstance(), &User::userIPChanged, this, &MainWindow::updateUserIP);
    connect(User::getInstance(), &User::teamTypeChanged, this, &MainWindow::updateTeamType);
}
void MainWindow::updateUserID(const QString& userID){
    ui->labelUserIDValue->setText(userID);
}   
void MainWindow::updateUserIP(const QString& userIP){
    // ui->labelUserIPValue->setText(userIP);
}
void MainWindow::updateTeamType(TeamType teamType){
    // ui->labelTeamValue->setText(User::getInstance()->getStrTeamType());
    if (User::getInstance()->getTeamType() == TeamType::Red){
        ui->labelTeamValue->setText("探員");
    }
    else if (User::getInstance()->getTeamType() == TeamType::Blue){
        ui->labelTeamValue->setText("指揮官");
    }
}
void MainWindow::updateUUID(const QString& uuid){
    ui->labelUUIDValue->setText(uuid);
}
void MainWindow::loadTheme()
{ 
    QFile file(":/darktheme.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        this->setStyleSheet(stream.readAll());
    }
}

void MainWindow::setupWidgets(QWidget *placeholder, QWidget *widget){
 
    // Create a layout for the placeholder
    QVBoxLayout *layout = new QVBoxLayout(placeholder);
    
    // Add the ChatRoom to the layout
    layout->addWidget(widget);
    
    // Set the layout to the placeholder
    placeholder->setLayout(layout);
}


