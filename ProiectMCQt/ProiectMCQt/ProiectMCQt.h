#ifndef PROIECTMCQT_H
#define PROIECTMCQT_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QLabel>

class ProiectMCQt : public QMainWindow {
    Q_OBJECT

public:
    ProiectMCQt(QWidget* parent = nullptr);
    ~ProiectMCQt();
    QWidget* centralWidget = nullptr; // Define centralWidget
    QTimer* bulletUpdateTimer = nullptr; // Define timer for bullets
    void setupUI(); // Declare setupUI
    void showLoginRegisterUI(); // Declare showLoginRegisterUI
    void updateBullets();
private slots:
    // Player management routes
    void registerUser(const QString& username, const QString& password);
    void loginUser(const QString& username, const QString& password);
    void joinGame(const QString& username);
    void checkMatchStatus(const QString& username);

    // Game state routes
    void fetchMapData();
    void sendMoveRequest(const QString& username, const QString& input);
    void sendShootRequest(const QString& username);
    void fetchBulletData();

    // Game upgrade routes
    void upgradeBulletSpeed(const QString& username);
    void upgradeFireRate(const QString& username);
    void fetchScoreAndPoints(const QString& username);
    void showUpgradesUI();

    void colorTile(QLabel* label, int type);
    void showGameMenuUI();
    void waitForMatch();
    void updateMapDisplay(const QJsonObject& mapData);

private:
    QString m_currentUsername;
    QString m_baseUrl = "http://localhost:18080";
    void handleNetworkError(const QString& operation, const QString& error);
};

#endif // PROIECTMCQT_H
