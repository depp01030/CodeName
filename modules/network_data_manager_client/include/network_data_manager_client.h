#ifndef NETWORK_DATA_MANAGER_CLIENT_H
#define NETWORK_DATA_MANAGER_CLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QNetworkAccessManager>

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray> 
#include "network_data_manager.h"
 
class Message;
class MessageProcesser;
 
class NetworkDataManagerClient : public NetworkDataManager
{
    Q_OBJECT

private:
    bool m_isConnected = false;
    QTcpSocket* m_socket;
    QNetworkAccessManager* m_networkManager;  
    void connectToServer(); 
    void sendMessage(std::shared_ptr<Message> message); 
    void receiveMessage(std::shared_ptr<Message> message);
public:
    // User* m_user;
    bool m_isLoggedIn; 
    NetworkDataManagerClient(QObject *parent = nullptr);
    ~NetworkDataManagerClient();
    // bool login(const QString& username, const QString& password);
    // void sendMessageToServer(const QString& message);
    QString getIPAddress() override;
    void broadcastMessage(std::shared_ptr<Message> message) override;  
private slots : 
    void onConnected();
    void onDisconnected();
public slots : 
    void onReadSocketMessage() override;
    void onBroadcastServerGameOperation(std::shared_ptr<Message> message) override;
    void onReceivingLocalMessage(std::shared_ptr<Message> message) override;
    void onReceivingSocketMessage(std::shared_ptr<Message> message) override;

};

#endif
    