#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
    
namespace Ui {
class MainWindow;
}
class NetworkDataManager;
class ChatRoom;
class GameController;
enum class TeamType;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    bool isServer = false;
    Ui::MainWindow *ui;
    NetworkDataManager *m_networkDataManager;
    ChatRoom *m_chatRoom;  
    GameController *m_gameController;
    void setupUserInfo();
    void setupUi();
    void loadTheme();
    void setupConnection();
    void setupWidgets(QWidget *placeholder, QWidget *widget);
private slots:
    void updateUUID(const QString& uuid);
    void updateUserID(const QString& userID);
    void updateUserIP(const QString& userIP);
    void updateTeamType(TeamType teamType);
};

#endif // MAINWINDOW_H