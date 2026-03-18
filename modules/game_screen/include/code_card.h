#ifndef CODE_CARD_H
#define CODE_CARD_H

#include <QPushButton>  
#include <QGraphicsDropShadowEffect>
#include <QEvent>
// #include "card_color.h"
class CodeCard : public QPushButton {
    Q_OBJECT

private:
    QGraphicsDropShadowEffect *m_shadowEffect = nullptr;
    QGraphicsBlurEffect *m_clearEffect = nullptr;
    QColor m_backgroundColor = QColor(87, 83, 72);
    QColor m_blurColor = m_backgroundColor;
public: 
    explicit CodeCard(int id, QString word, QWidget *parent = nullptr);
    // void setupConnections();
    int m_id;
    QString m_word; 
    void turnOpen(const QColor &color);
    void setCommanderStyle(const QColor &color);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
signals:
    void cardClicked(const CodeCard* codeCard);
private: 
    void initializeCard(); 
};
#endif // CODE_CARD_H