#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QColor>

namespace Ui {
    class ChatRoom;
}
class NetworkDataManager;
class ChatText;
class Message;

class ChatRoom : public QWidget
{
    Q_OBJECT
private:
    NetworkDataManager *m_networkDataManager;
    std::unordered_map<QString, QColor> m_usernameColorMap;
public:
    explicit ChatRoom(QWidget *parent = nullptr,
     NetworkDataManager *networkDataManager = nullptr);
    explicit ChatRoom(QWidget *parent = nullptr);
    ~ChatRoom();
 
private slots:
    void onSendButtonClicked();
    void handleChatMessageFromServer(std::shared_ptr<Message> message);
signals:
    void sendChatMessageToServer(std::shared_ptr<Message> message);
private:
    Ui::ChatRoom *ui;
    NetworkDataManager *m_server;
    void setupConnection();
    QString colorText(const QString& text, const QColor& color);
    QColor getColor(const QString& username);
    void addChatText(const ChatText& chatText);
    void handleChatMessage(ChatText &chatText);
    // bool is_command(ChatTexmt &chatText);
    void handleCommand(ChatText &chatText);
    // void connectChatTextEdit();

};

#endif // CHATROOM_H
