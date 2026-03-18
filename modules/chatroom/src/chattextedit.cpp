#include "../include/chattextedit.h"

ChatTextEdit::ChatTextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void ChatTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (event->modifiers() & Qt::ShiftModifier) {
            // Shift + Enter: 插入新行
            QTextEdit::keyPressEvent(event);
        } else {
            // 只有 Enter: 發送消息
            emit enterPressed();
            event->accept();
        }
    } else {
        QTextEdit::keyPressEvent(event);
    }
}