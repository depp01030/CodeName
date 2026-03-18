#include <QPainter>
#include <QDebug>
#include "../include/game_screen.h"
#include "code_card.h"
#include <QEvent>
#include <QGraphicsBlurEffect>


CodeCard::CodeCard(int id, QString word, QWidget *parent):
    m_id(id), m_word(word),
    QPushButton(parent) 
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setColor(m_blurColor);
    m_shadowEffect->setBlurRadius(0);
    m_shadowEffect->setOffset(0, 0);
    initializeCard();  
 
}
void CodeCard::initializeCard()
{
    setText(m_word); 
    setStyleSheet("background-color: " + m_backgroundColor.name() + "; color: white; border: 3px solid " + m_backgroundColor.name() + ";");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
void CodeCard::enterEvent(QEnterEvent *event) {
    
    m_shadowEffect->setBlurRadius(30);
    this->setGraphicsEffect(m_shadowEffect);  // Apply the existing effect

    QPushButton::enterEvent(event);
}

void CodeCard::leaveEvent(QEvent *event) {
    m_shadowEffect->setBlurRadius(0);
    // this->setGraphicsEffect(m_shadowEffect);  // Remove effect, but don't delete it

    QPushButton::leaveEvent(event);
}


void CodeCard::turnOpen(const QColor &color){
    m_shadowEffect->setColor(color);
    setStyleSheet("background-color: " + color.name() + "; color: white;"); 
}
void CodeCard::setCommanderStyle(const QColor &color){
    m_shadowEffect->setColor(color);
    setStyleSheet(" border: 3px solid " + color.name() + ";");
}