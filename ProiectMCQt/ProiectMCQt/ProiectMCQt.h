#ifndef PROIECTMCQT_H
#define PROIECTMCQT_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QLabel>

class ProiectMCQt : public QMainWindow
{
    Q_OBJECT

public:
    ProiectMCQt(QWidget* parent = nullptr);
    ~ProiectMCQt();

private slots:
    void fetchData();
    //void colorWall(QLabel* label, int type);
    void colorTile(QLabel* label, int type);
    void keyPressEvent(QKeyEvent* event);
    void sendMoveRequest(int x, int y, int playerID);
    void registerUser(const std::string& username, const std::string& password);
    void loginUser(const std::string& username, const std::string& password);
    //void displayMenu();
    void joinGame();
    void waitForMatch();
    void showLoginRegisterUI();
    void showGameMenuUI();
    void showGameUI();
    void shootBullet(int playerID, const std::string& direction, int x, int y, int speed);



private:
    QWidget* centralWidget;
    struct Bullet {
        int x;
        int y;
        std::string direction;
        int playerID;
    };
    std::vector<Bullet> bullets;
    QTimer* bulletUpdateTimer;
    void updateBullets();
    //QGridLayout* layout;

    void setupUI(); // Noua metodă pentru configurarea UI-ului
    std::pair<int, int> playerPosition = { 1, 1 };
    int playerID = 1;
    // QWidget* centralWidget;
    std::string username;
};

#endif // PROIECTMCQT_H
