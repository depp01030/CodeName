#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
// #include <QNetworkAccessManager>
// #include <QNetworkRequest>
#include <QNetworkReply> 
#include <Qstring>
#include <QEventLoop>

#include "game_state.h"
#include "message_processer.h"
#include "message.h"
#include "chat_message.h"
#include "system_message.h"
#include "user.h"
#include "game_message.h"
#include "network_data_manager_server.h"

NetworkDataManagerServer::NetworkDataManagerServer(QObject *parent) 
    : NetworkDataManager(parent), 
      m_server(new QTcpServer(this)), 
      m_httpServer(new QHttpServer(this))
{ 
    startServer(); 
    startHttpServer(); 
}  
NetworkDataManagerServer::~NetworkDataManagerServer(){
    delete m_server;
    delete m_httpServer;
    delete m_networkManager;
}
void NetworkDataManagerServer::startServer(){
    if(m_server->isListening()){
        return;
    }
    m_server->listen(QHostAddress("127.0.0.1"), 1234);
    connect(m_server, &QTcpServer::newConnection, this, &NetworkDataManagerServer::onNewConnection);
}
QString NetworkDataManagerServer::getIPAddress() {
    return m_server->serverAddress().toString();
}
void NetworkDataManagerServer::startHttpServer(){
    // m_httpServer->route("/login", QHttpServerRequest::Method::Post,
    // [this](const QHttpServerRequest &request){
    //     return handleHttpRequest(request);
        
    // });
    // m_httpServer->listen(QHostAddress("127.0.0.1"), 8080);
} 

void NetworkDataManagerServer::onNewConnection() {
    QTcpSocket* client_socket = m_server->nextPendingConnection();
    connect(client_socket, &QTcpSocket::readyRead, this, &NetworkDataManagerServer::onReadSocketMessage);
    connect(client_socket, &QTcpSocket::disconnected, this, &NetworkDataManagerServer::onDisconnected);
    QString userUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    qDebug() << "New connection : " << userUuid;
    m_userMap[userUuid] = client_socket;

    // send user login message to client
    std::shared_ptr<SystemMessage> systemMessage = std::make_shared<SystemMessage>(SystemCommandType::SetUserUUID, userUuid); 
    std::shared_ptr<Message> message = m_messageProcesser->messageWrapper(systemMessage);  
    client_socket->write(message->toQByteArray());


    
}   
void NetworkDataManagerServer::onDisconnected() {
    // do nothing. 
}
void NetworkDataManagerServer::onBroadcastServerGameOperation(std::shared_ptr<Message> message) {
    // SERVER ONLY : to broadcast the message to all clients. 
    try{
        qDebug() << "onBroadcastServerGameOperation";
        message = m_messageProcesser->messageWrapper(message);  
        broadcastMessage(message);
    }catch(const std::exception& e){
        qDebug() << "Error: " << e.what();
    }
}
void NetworkDataManagerServer::onReceivingLocalMessage(std::shared_ptr<Message> message) {
    try{ 
        qDebug() << "onSendingMessageToServer";
        message = m_messageProcesser->messageWrapper(message);  
        m_messageProcesser->handleMessage(message);

        if (m_messageProcesser->needBroadcast(message)){
            broadcastMessage(message);
        }
    }catch(const std::exception& e){
        qDebug() << "Error: " << e.what();
    }
}

void NetworkDataManagerServer::onReceivingSocketMessage(std::shared_ptr<Message> message) {
    receiveMessage(message);    
    if (m_messageProcesser->needBroadcast(message)){
        broadcastMessage(message);
    }
}

void NetworkDataManagerServer::broadcastMessage(std::shared_ptr<Message> message)
{ 
    // message = m_messageProcesser->messageWrapper(message);   
    QByteArray byteMessage = m_messageProcesser->messageObjToQByteArray(message); 
    for (auto &client: m_userMap){
        qDebug() << "send message to client :"  << client.first.mid(0,10);
        qDebug() << "message : " << message->toJson();
        client.second->write(byteMessage);
    }
}

void NetworkDataManagerServer::sendMessage(std::shared_ptr<Message> message) {
    qDebug() << "sendMessage : " << message->toJson(); 
    QByteArray byteMessage = m_messageProcesser->messageObjToQByteArray(message);
    // m_socket->write(byteMessage);  
}
void NetworkDataManagerServer::onReadSocketMessage()
{ 
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender()); 
    if (!socket) {
        std::cout << "socket is null" << std::endl;
        return; 
    }
    qDebug() << "onMessageReceivedFromServer";
    QByteArray data = socket->readAll();
    std::vector<std::shared_ptr<Message>> messages = m_messageProcesser->processSocketMessage(data); 
    for (auto &message : messages) {
        onReceivingSocketMessage(message);
    }
}
void NetworkDataManagerServer::receiveMessage(std::shared_ptr<Message> message) {
    qDebug() << "receiveMessage SERVER";
    m_messageProcesser->handleMessage(message);
         
} 
