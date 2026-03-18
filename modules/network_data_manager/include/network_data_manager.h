#ifndef NETWORK_DATA_MANAGER_H
#define NETWORK_DATA_MANAGER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QNetworkAccessManager>

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray> 
 
 
class Message;
class MessageProcesser; 

class NetworkDataManager : public QObject
{
    Q_OBJECT

protected:
    QNetworkAccessManager* m_networkManager; 
    MessageProcesser* m_messageProcesser;
    // void connectToServer(); 
    // void sendMessage(std::shared_ptr<Message> message); 
    // void receiveMessage(std::shared_ptr<Message> message);
public: 
    bool m_isLoggedIn; 
    explicit NetworkDataManager(QObject *parent = nullptr);
    virtual ~NetworkDataManager(){}; 
    virtual void broadcastMessage(std::shared_ptr<Message> message){};
    virtual QString getIPAddress() = 0;
signals:
    // virtual void loginResult(bool success, const QString& message) = 0;
    void receiveChatMessageFromServer(std::shared_ptr<Message> chatMessage);
    void receiveOperationsFromServer(std::shared_ptr<Message> message);
public slots :  
    virtual void onReadSocketMessage() = 0;
    virtual void onBroadcastServerGameOperation(std::shared_ptr<Message> message) = 0;
    virtual void onReceivingSocketMessage(std::shared_ptr<Message> message) = 0;
    virtual void onReceivingLocalMessage(std::shared_ptr<Message> message) = 0;

};

#endif
    