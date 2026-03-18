#include <iostream>
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

#include "network_data_manager.h" 
 
NetworkDataManager::NetworkDataManager(QObject *parent)
    : QObject(parent),  
      m_messageProcesser(new MessageProcesser(this)),
      m_networkManager(new QNetworkAccessManager(this))
{
    // do nothing. 
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