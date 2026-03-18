#ifndef NETWORK_DATA_MANAGER_SERVER_H
#define NETWORK_DATA_MANAGER_SERVER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer> 
#include <QtHttpServer/QtHttpServer> // Added for QHttpServer
#include <QNetworkAccessManager>

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray> 
#include "network_data_manager.h"
class Message;
class MessageProcesser; 
class NetworkDataManagerServer : public NetworkDataManager
{
    Q_OBJECT

private:
    QTcpServer* m_server;
    QHttpServer* m_httpServer;
    QNetworkAccessManager* m_networkManager; 

    std::map<QString, QTcpSocket*> m_userMap; // socket, IP
    
    void startServer();
    void startHttpServer(); 
    
    void sendMessage(std::shared_ptr<Message> message); 
    void receiveMessage(std::shared_ptr<Message> message);
public: 
    NetworkDataManagerServer(QObject *parent = nullptr);
    ~NetworkDataManagerServer();
    QString getIPAddress() override;
    void broadcastMessage(std::shared_ptr<Message> message);

private slots : 
    void onNewConnection(); 
    // void onConnected();
    void onDisconnected();
public slots : 
    void onReadSocketMessage() override;
    void onBroadcastServerGameOperation(std::shared_ptr<Message> message) override;
    void onReceivingLocalMessage(std::shared_ptr<Message> message) override;
    void onReceivingSocketMessage(std::shared_ptr<Message> message) override;

};

#endif
