#include "user.h"
#include <QRandomGenerator>
// Initialize the static instance pointer

User* User::instance = nullptr;
void User::setInstance(const QString& userID, const QString& userIP) {
    if (instance == nullptr) {
        instance = new User(userID, userIP, nullptr);
    }
} 
void User::setInstance(const QString& userIP) {
    QString userID = QString("Vistor_") + QString::number(QRandomGenerator::global()->bounded(1000000));
    if (instance == nullptr) {
        instance = new User(userID, userIP, nullptr);
    }
} 
User* User::getInstance() {
    return instance;
} 