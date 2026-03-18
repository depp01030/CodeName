#include <iostream>
#include <QThread>
// #include <QNetworkAccessManager>
// #include <QNetworkRequest>
#include <QNetworkReply>
#include <Qstring>
#include <QEventLoop> 

#include "game_state.h"
#include "message_processer.h"
#include "message.h"
#include "chat_message.h"
#include "user.h"
#include "game_message.h"

#include "network_data_manager_client.h" 
 
NetworkDataManagerClient::NetworkDataManagerClient(QObject *parent)
    : NetworkDataManager(parent),
      m_socket(new QTcpSocket(this)),  
      m_isLoggedIn(false)
    //   m_user()
{ 
    connectToServer();
    connect(m_socket, &QTcpSocket::connected, this, &NetworkDataManagerClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkDataManagerClient::onReadSocketMessage);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkDataManagerClient::onDisconnected); 
    // Constructor implementation
}
NetworkDataManagerClient::~NetworkDataManagerClient()
{
    if (m_socket) {
        m_socket->disconnectFromHost();
        delete m_socket;
    }
    if (m_networkManager) {
        delete m_networkManager;
    }
}
void NetworkDataManagerClient::onConnected() {
    // Handle new connection
    std::cout << "Connected to server" << std::endl;
    m_isConnected = true;
    

}
void NetworkDataManagerClient::connectToServer() {
    m_socket->connectToHost("127.0.0.1", 1234);  
}

void NetworkDataManagerClient::onDisconnected() {
    // Handle disconnected
}  

QString NetworkDataManagerClient::getIPAddress() {
    return m_socket->peerAddress().toString();
}


void NetworkDataManagerClient::onBroadcastServerGameOperation(std::shared_ptr<Message> message) {
    // do nothing.  
    // QThread::sleep(10);
}
void NetworkDataManagerClient::onReceivingLocalMessage(std::shared_ptr<Message> message) {  
    if (!m_isConnected && User::getInstance()->getUUID().isEmpty()){
        QString userUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
        User::getInstance()->setUUID(userUuid); 
    }
    message = m_messageProcesser->messageWrapper(message);   
    sendMessage(message);
} 
void NetworkDataManagerClient::sendMessage(std::shared_ptr<Message> message) { 
    QByteArray byteMessage = m_messageProcesser->messageObjToQByteArray(message); 
    m_socket->write(byteMessage);   
    // handle it's self. 
    if (!m_isConnected) {
        m_messageProcesser->handleMessage(message); 
    }
 
} 
void NetworkDataManagerClient::onReadSocketMessage() {
    qDebug() << "onMessageReceivedFromServer";
    QByteArray data = m_socket->readAll();
    std::vector<std::shared_ptr<Message>> messages = m_messageProcesser->processSocketMessage(data); 
    for (auto &message : messages) {
        onReceivingSocketMessage(message);
        // broadcastMessage(message);
    }
}
void NetworkDataManagerClient::onReceivingSocketMessage(std::shared_ptr<Message> message) {
    receiveMessage(message);    
}

void NetworkDataManagerClient::receiveMessage(std::shared_ptr<Message> message) {
    m_messageProcesser->handleMessage(message); 
         
} 


void NetworkDataManagerClient::broadcastMessage(std::shared_ptr<Message> message){
    return;
}

// bool NetworkDataManager::login(const QString& username, const QString& password) {
//     QJsonObject loginData;
//     loginData["username"] = username;
//     loginData["password"] = password;
//     QJsonDocument doc(loginData);

//     QNetworkRequest request(QUrl("http://127.0.0.1:8080/login"));
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//     QNetworkReply* reply = m_networkManager->post(request, doc.toJson());
    
//     QEventLoop loop;
//     connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//     loop.exec();

//     QByteArray response = reply->readAll();
//     std::cout << "Response: " << response.toStdString() << std::endl;
//     QJsonObject responseObj = QJsonDocument::fromJson(response).object();
//     m_isLoggedIn = responseObj["success"].toBool();
//     std::cout << "Login result: " << m_isLoggedIn << std::endl;
//     return m_isLoggedIn;
// }