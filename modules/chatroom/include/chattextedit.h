#ifndef CHATTEXTEDIT_H
#define CHATTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>

class ChatTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit ChatTextEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void enterPressed();
};

#endif // CHATTEXTEDIT_H