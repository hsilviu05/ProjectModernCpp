#include "ProiectMCQt.h"
#include <cpr/cpr.h>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPixmap>
#include <QDebug>
#include <QBrush>
#include <QGraphicsOpacityEffect>
#include<QLineEdit>
#include<QPushButton>
#include<QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QUrlQuery>

ProiectMCQt::ProiectMCQt(QWidget* parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    SetupUI();
}

ProiectMCQt::~ProiectMCQt()
{
}

void ProiectMCQt::SetupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    centralWidget->setLayout(layout);

    ShowLoginRegisterUI();

}

void ProiectMCQt::ShowLoginRegisterUI()
{
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QLabel* usernameLabel = new QLabel("Username:", this);
    QLineEdit* usernameLineEdit = new QLineEdit(this);
    layout->addWidget(usernameLabel, 0, 0);
    layout->addWidget(usernameLineEdit, 0, 1);

    QLabel* passwordLabel = new QLabel("Password:", this);
    QLineEdit* passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLabel, 1, 0);
    layout->addWidget(passwordLineEdit, 1, 1);

    QPushButton* registerButton = new QPushButton("Register", this);
    QPushButton* loginButton = new QPushButton("Login", this);
    layout->addWidget(registerButton, 2, 0);
    layout->addWidget(loginButton, 2, 1);

    QObject::connect(registerButton, &QPushButton::clicked, [=]() {
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();
        RegisterUser(username, password);
        });

    QObject::connect(loginButton, &QPushButton::clicked, [=]() {
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();
        LoginUser(username, password);
        });
}

void ProiectMCQt::ShowUpgradesUI()
{
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel* upgradesLabel = new QLabel("Upgrades", this);
    QPushButton* upgradeBulletSpeedButton = new QPushButton("Upgrade Bullet Speed", this);
    QPushButton* upgradeFireRateButton = new QPushButton("Upgrade Fire Rate", this);
    QLabel* scoreLabel = new QLabel("Score: 0", this);
    QLabel* pointsLabel = new QLabel("Points: 0", this);
    QPushButton* backButton = new QPushButton("Back to Menu", this);

    upgradesLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = upgradesLabel->font();
    titleFont.setPointSize(16);
    upgradesLabel->setFont(titleFont);

    layout->addWidget(upgradesLabel, 0, 0, 1, 2);
    layout->addWidget(scoreLabel, 1, 0, 1, 2);
    layout->addWidget(pointsLabel, 2, 0, 1, 2);
    layout->addWidget(upgradeBulletSpeedButton, 3, 0, 1, 2);
    layout->addWidget(upgradeFireRateButton, 4, 0, 1, 2);
    layout->addWidget(backButton, 5, 0, 1, 2);

    layout->setAlignment(Qt::AlignCenter);

    connect(upgradeBulletSpeedButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            UpgradeBulletSpeed(m_currentUsername);
        }
        });

    connect(upgradeFireRateButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            UpgradeFireRate(m_currentUsername);
        }
        });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        ShowGameMenuUI();
        });

    if (!m_currentUsername.isEmpty()) {
        FetchScoreAndPoints(m_currentUsername);
    }
}

void ProiectMCQt::UpdateBullets()
{

}

void ProiectMCQt::RegisterUser(const QString& username, const QString& password) {
    QNetworkRequest request(QUrl(m_baseUrl + "/signup"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    jsonObj["password"] = password;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Success", "Account created successfully!");
        }
        else {
            HandleNetworkError("Registration", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::LoginUser(const QString& username, const QString& password) {
    QNetworkRequest request(QUrl(m_baseUrl + "/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    jsonObj["password"] = password;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this, username](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            m_currentUsername = username;
            QMessageBox::information(this, "Success", "Login successful!");
            ShowGameMenuUI();
        }
        else {
            HandleNetworkError("Login", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::JoinGame(const QString& username) {

    if (username.isEmpty()) {
        qDebug() << "Username is empty!";
        HandleNetworkError("Join Game", "Username cannot be empty.");
        return;
    }

    QNetworkRequest request(QUrl(m_baseUrl + "/join_game"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    qDebug() << "Request JSON:" << doc.toJson(QJsonDocument::Compact);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this, username](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            switch (statusCode) {
            case 200: {
                qDebug() << "Player" << username << "successfully joined the game.";
                WaitForMatch();
                break;
            }
            case 400: {
                QString errorMsg = reply->readAll();
                qDebug() << "Failed to join game:" << errorMsg;
                HandleNetworkError("Join Game", errorMsg);
                break;
            }
            case 404: {
                qDebug() << "Player" << username << "not found in lobby.";
                HandleNetworkError("Join Game", "Player not found in lobby.");
                break;
            }
            default: {
                qDebug() << "Unexpected server response:" << reply->readAll();
                HandleNetworkError("Join Game", "Unexpected server response.");
                break;
            }
            }
        }
        else {
            qDebug() << "Network error occurred:" << reply->errorString();
            HandleNetworkError("Join Game", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
    
}

void ProiectMCQt::CheckMatchStatus(const QString& username)
{
}

void ProiectMCQt::SendMoveRequest(const QString& username, const QString& input) {
    QNetworkRequest request(QUrl(m_baseUrl + "/move"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    jsonObj["input"] = input;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
        }
        else {
           HandleNetworkError("Move", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::SendShootRequest(const QString& username) {
    QNetworkRequest request(QUrl(m_baseUrl + "/shoot"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Shoot request successful.";
            
        }
        else {
            HandleNetworkError("Shoot", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}


void ProiectMCQt::keyPressEvent(QKeyEvent* event)
{
    std::string input;

    switch (event->key()) {
    case Qt::Key_W:
        input = "W";
        break;
    case Qt::Key_S:
        input = "S";
        break;
    case Qt::Key_A:
        input = "A";
        break;
    case Qt::Key_D:
        input = "D";
        break;
    case Qt::Key_Space:
        SendShootRequest(m_currentUsername);
    default:
        return;
    }

    SendMoveRequest( m_currentUsername, QString::fromStdString(input));
}

void ProiectMCQt::FetchBulletData()
{
    QNetworkRequest request(QUrl(m_baseUrl + "/get_bullet_data"));
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject responseObj = jsonResponse.object();
        }
        else {
            HandleNetworkError("Fetch Bullet Data", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::UpgradeBulletSpeed(const QString& username)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/upgrade_bullet_speed"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
        }
        else {
            HandleNetworkError("Upgrade Bullet Speed", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::UpgradeFireRate(const QString& username)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/upgrade_fire_rate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
        }
        else {
            HandleNetworkError("Upgrade Fire Rate", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::FetchScoreAndPoints(const QString& username)
{
    QUrl url(m_baseUrl + "/get_total_score_and_points");
    QUrlQuery query;
    query.addQueryItem("username", username);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject responseObj = jsonResponse.object();
        }
        else {
            HandleNetworkError("Fetch Score and Points", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::FetchMapData() {
    QNetworkRequest request(QUrl(m_baseUrl + "/map"));

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            if (!doc.isNull()) {
                UpdateMapDisplay(doc.object());
            }
        }
        else {
            HandleNetworkError("Fetch Map", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::HandleNetworkError(const QString& operation, const QString& error) {
    QMessageBox::critical(this, operation + " Error",
        "An error occurred during " + operation.toLower() + ": " + error);
}

QMap<QString, QPixmap> imageCache;
QPixmap loadImage(const QString& path) {
    if (imageCache.contains(path)) {
        return imageCache.value(path);
    }
    else {
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            imageCache.insert(path, pixmap);
        }
        return pixmap;
    }
}

void ProiectMCQt::ColorTile(QLabel* label, int type)
{
    QPixmap pixmap;
    QPixmap backgroundPixmap(":/images/Untitled.png");
    switch (type) {
    case 0:
        label->setAutoFillBackground(false);
        label->clear();
        break;
    case 1:
        pixmap = loadImage(":/images/poza4.png");
        if (pixmap.isNull()) {
            qDebug() << "Failed to load image!";
        }
        else {
            qDebug() << "Image loaded successfully!";
            label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        break;
    case 2: 
        pixmap = loadImage(":/images/pngwing.com.png");
        label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        break;
    case 3:
        pixmap = loadImage(":/images/poza4.png");
        label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        break;
    case 4:
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::yellow));
        break;
    case 5:
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::white));
        label->setFixedSize(10, 10);
        break;
    default:
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::gray));
        break;
    }
}

void ProiectMCQt::ShowGameMenuUI() {
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel* menuLabel = new QLabel("Game Menu", this);
    QPushButton* joinGameButton = new QPushButton("Join Game", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);
    QPushButton* upgradesButton = new QPushButton("Upgrades", this);

    menuLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = menuLabel->font();
    titleFont.setPointSize(16);
    menuLabel->setFont(titleFont);

    layout->addWidget(menuLabel, 0, 0, 1, 2);
    layout->addWidget(joinGameButton, 1, 0, 1, 2);
    layout->addWidget(upgradesButton, 2, 0, 1, 2);
    layout->addWidget(logoutButton, 3, 0, 1, 2);

    connect(joinGameButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            JoinGame(m_currentUsername);
        }
        });

    connect(logoutButton, &QPushButton::clicked, this, [this]() {
        m_currentUsername.clear();
        ShowLoginRegisterUI();
        });

    connect(upgradesButton, &QPushButton::clicked, this, [this]() {
        ShowUpgradesUI();
        });

    layout->setAlignment(Qt::AlignCenter);
}

void ProiectMCQt::WaitForMatch() {

    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel* waitingLabel = new QLabel("Waiting for other players...", this);
    waitingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(waitingLabel, 0, 0);

    QTimer* matchTimer = new QTimer(this);
    connect(matchTimer, &QTimer::timeout, this, [this, matchTimer]() {
        QNetworkRequest request(QUrl(m_baseUrl + "/game_started"));

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, [this, matchTimer](QNetworkReply* reply) {
            if (reply->error() == QNetworkReply::NoError) {
                int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

                if (statusCode == 200) {
                    matchTimer->stop();
                    matchTimer->deleteLater();

                    if (!m_mapUpdateTimer) {
                        m_mapUpdateTimer = new QTimer(this);
                        connect(m_mapUpdateTimer, &QTimer::timeout, this, &ProiectMCQt::FetchMapData);
                    }
                    m_mapUpdateTimer->start(2000);

                    FetchMapData();
                }
            }
            reply->deleteLater();
            sender()->deleteLater();
            });

        manager->get(request);
        });

    matchTimer->start(2000);
}

void ProiectMCQt::UpdateMapDisplay(const QJsonObject& mapData) {
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());

    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    int height = mapData["height"].toInt();
    int width = mapData["width"].toInt();
    QJsonArray mapArray = mapData["map"].toArray();

    for (int i = 0; i < height; ++i) {
        QJsonArray row = mapArray[i].toArray();
        for (int j = 0; j < width; ++j) {
            int tileType = row[j].toInt();
            QLabel* tileLabel = new QLabel(this);
            tileLabel->setFixedSize(30, 30);
            ColorTile(tileLabel, tileType);
            layout->addWidget(tileLabel, i, j);
        }
    }

    QJsonArray wallsArray = mapData["walls"].toArray();
    for (const QJsonValue& wallValue : wallsArray) {
        QJsonObject wall = wallValue.toObject();
        int x = wall["x"].toInt();
        int y = wall["y"].toInt();
        int type = wall["type"].toInt();

        QLayoutItem* item = layout->itemAtPosition(x, y);
        if (item && item->widget()) {
            QLabel* wallLabel = qobject_cast<QLabel*>(item->widget());
            if (wallLabel) {
                ColorTile(wallLabel, type);
            }
        }
    }

    layout->setSpacing(1);
    layout->setContentsMargins(10, 10, 10, 10);
 }