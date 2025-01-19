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



protected:
    void keyPressEvent(QKeyEvent* event) override;

public:
    ProiectMCQt(QWidget* parent = nullptr);
    ~ProiectMCQt();
    QWidget* centralWidget = nullptr;
    QTimer* bulletUpdateTimer = nullptr;
    void SetupUI();
    void ShowLoginRegisterUI();
    void UpdateBullets();
private slots:
    void RegisterUser(const QString& username, const QString& password);
    void LoginUser(const QString& username, const QString& password);
    void JoinGame(const QString& username);
    void CheckMatchStatus(const QString& username);

    void FetchMapData();
    void SendMoveRequest(const QString& username, const QString& input);
    void SendShootRequest(const QString& username);
    void FetchBulletData();

    void UpgradeBulletSpeed(const QString& username);
    void UpgradeFireRate(const QString& username);
    void FetchScoreAndPoints(const QString& username);
    void ShowUpgradesUI();

    void ColorTile(QLabel* label, int type);
    void ShowGameMenuUI();
    void WaitForMatch();
    void UpdateMapDisplay(const QJsonObject& mapData);

private:
    QTimer* m_mapUpdateTimer=nullptr;
    QString m_currentUsername;
    QString m_baseUrl = "http://localhost:18080";
    void HandleNetworkError(const QString& operation, const QString& error);
};

#endif // PROIECTMCQT_H
