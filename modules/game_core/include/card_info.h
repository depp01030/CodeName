#ifndef CARD_INFO_H
#define CARD_INFO_H
#include <QColor> 
#include "utils/include/color.h"
enum class CardType{
    RedSpy,
    BlueSpy,
    GreenSpy,
    People,
    Mine, 
};
enum class CardState{
    Open,
    Close,
};

inline QColor getCardColor(const CardType cardType) {
    switch (cardType) {
        case CardType::RedSpy:
            return color::RED_TEAM;
        case CardType::BlueSpy:
            return color::BLUE_TEAM;
        case CardType::GreenSpy:
            return QColor(70, 184, 108);
        case CardType::People:  
            return QColor(38, 32, 60);
        case CardType::Mine:
            return QColor(219, 185, 89);
        default:
            return QColor(255, 255, 255);
    }
} 

class CardInfo {
public:
    QString m_word;
    bool m_isAnswer; 
    int m_id;
    size_t m_index;
    CardState m_state;
    CardType m_cardType;
    QColor m_answerColor;
    CardInfo(const QString& word, CardType cardType, bool is_answer = false):
    m_word(word), m_cardType(cardType), m_isAnswer(is_answer), m_state(CardState::Close) 
    { 
        m_answerColor = getCardColor(m_cardType);
    }
    QJsonObject toJson() const {
        QJsonObject jsonObj;
        jsonObj["word"] = m_word;
        jsonObj["isAnswer"] = m_isAnswer;
        jsonObj["id"] = m_id;
        jsonObj["index"] = static_cast<int>(m_index);
        jsonObj["state"] = static_cast<int>(m_state);
        jsonObj["cardType"] = static_cast<int>(m_cardType);
        return jsonObj;
    }
    static CardInfo fromQJsonObject(const QJsonObject& jsonObj){
        CardInfo cardInfo(jsonObj["word"].toString(), static_cast<CardType>(jsonObj["cardType"].toInt()), jsonObj["isAnswer"].toBool());
        cardInfo.m_id = jsonObj["id"].toInt();
        cardInfo.m_index = jsonObj["index"].toInt();
        cardInfo.m_state = static_cast<CardState>(jsonObj["state"].toInt());
        return cardInfo;
    }
private:
};
 


#endif // CARD_INFO_H