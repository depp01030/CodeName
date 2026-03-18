#ifndef CHAT_TEXT_H
#define CHAT_TEXT_H

#include <QColor>
#include <QString>
#include "chat_message.h"
class ChatText {
private: 
public: 
    QString m_content;
    QString m_userID;
    ChatText(const QString& content, const QString& userID)
        :  m_content(content),
           m_userID(userID){}
    ChatText(const std::shared_ptr<ChatMessage>& chatMessage)
        :  m_content(chatMessage->content),
           m_userID(chatMessage->userID){}

    QString getFormattedMessage() const {
        return QString("%1 : %2")
                    .arg(m_userID)
                    .arg(m_content);
    }
};
#endif // CHAT_TEXT_H   