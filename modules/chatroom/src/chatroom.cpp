#include "chatroom.h"
#include "ui_chatroom.h"
#include "chattextedit.h"
#include "chat_text.h" 
#include "chat_message.h"
#include "network_data_manager.h"
#include "message.h" 
#include "color.h"
#include "system_message.h"
ChatRoom::ChatRoom(QWidget *parent, NetworkDataManager *networkDataManager)
    : QWidget(parent)
    , ui(new Ui::ChatRoom)
    , m_networkDataManager(networkDataManager)
{
    
    ui->setupUi(this);
    setupConnection();
    
}
ChatRoom::~ChatRoom()
{
    delete ui;
}
void ChatRoom::setupConnection(){
    // Connect the button's clicked signal to the onButtonClicked slot
    connect(ui->pushButton_send, &QPushButton::clicked, this, &ChatRoom::onSendButtonClicked);
    connect(ui->textEdit_message, &ChatTextEdit::enterPressed, this, &ChatRoom::onSendButtonClicked); 
    
    connect(this, &ChatRoom::sendChatMessageToServer, m_networkDataManager, &NetworkDataManager::onReceivingLocalMessage);
    connect(m_networkDataManager, &NetworkDataManager::receiveChatMessageFromServer, this, &ChatRoom::handleChatMessageFromServer);
    // connect(m_server, QOverload<const MessageData&>::of(&NetworkDataManager::messageReceivedFromServer), 
    //         this, QOverload<const MessageData&>::of(&ChatRoom::onUpdateChatBox));   
 
}
 
void ChatRoom::onSendButtonClicked()
{    
    QString input_text = ui->textEdit_message->toPlainText();
    
    if (SystemMessageFactory::isCommand(input_text)){
        std::shared_ptr<Message> systemMessage = SystemMessageFactory::createSystemMessage(input_text);
        emit sendChatMessageToServer(systemMessage); 

    }
    else{ 
        std::shared_ptr<Message> chatMessage = ChatMessageFactory::createChatMessage(input_text);
        emit sendChatMessageToServer(chatMessage); 
    }

    ui->textEdit_message->clear();
} 
 
 
void ChatRoom::handleChatMessageFromServer(std::shared_ptr<Message> message) {
    try { 
        qDebug() << "ChatRoom::handleChatMessageFromServer";
        std::shared_ptr<ChatMessage> chatMessage = std::dynamic_pointer_cast<ChatMessage>(message); 
        if (!chatMessage) {
            qDebug() << "Failed to cast message to ChatMessage";
            return;
        } 

        addChatText(ChatText(chatMessage));
        
    }
    catch (const std::exception& e) {
        qDebug() << "Exception: " << e.what();
    }
}

void ChatRoom::addChatText(const ChatText& chatText) {

    ui->textBrowser_chat_window->append(
        colorText(chatText.getFormattedMessage(), getColor(chatText.m_userID))
        );
}
QColor ChatRoom::getColor(const QString& userID) {
    if (m_usernameColorMap.find(userID) != m_usernameColorMap.end()){
        return m_usernameColorMap[userID];
    }
    else {
        QColor color(color::getRandomColor());
        m_usernameColorMap[userID] = color;
        return color;
    }
}

QString ChatRoom::colorText(const QString& text, const QColor& color) {
    return QString("<font color='%1'>%2</font>")
                    .arg(color.name()) 
                    .arg(text);
}
