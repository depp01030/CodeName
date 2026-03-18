#ifndef MESSAGE_PROCESSER_H
#define MESSAGE_PROCESSER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>
class Message;
class GameMessage;
class ChatMessage;
class NetworkDataManager;
class MessageProcesser{
private:
public:
    NetworkDataManager* m_networkDataManager;
    MessageProcesser(NetworkDataManager *networkDataManager);
    std::vector<std::shared_ptr<Message>> processSocketMessage(const QByteArray& data);
    std::shared_ptr<Message> jsonToMessage(const QJsonObject& messageObject);
    std::shared_ptr<GameMessage> createGameMessage(const QJsonObject& messageObject);
    std::shared_ptr<Message> messageWrapper(std::shared_ptr<Message> message);
    QByteArray messageObjToQByteArray(std::shared_ptr<Message> message);
    bool needBroadcast(std::shared_ptr<Message> message);
    void handleMessage(std::shared_ptr<Message> message);
    void handleCommandMessage(std::shared_ptr<Message> message);
    void handleChatMessage(std::shared_ptr<Message> message);
    void handleGameMessage(std::shared_ptr<Message> message);
    void handleSystemMessage(std::shared_ptr<Message> message);
};
#endif // MESSAGE_PROCESSER_H
 