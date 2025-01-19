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
    setAttribute(Qt::WA_TranslucentBackground); // Permite transparența pentru fereastra principală
    setupUI();
    //fetchData();
}

ProiectMCQt::~ProiectMCQt()
{
    // Curățarea resurselor, dacă este necesar
}

void ProiectMCQt::setupUI()
{
    // Create central widget and layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    centralWidget->setLayout(layout);

    // Initialize the UI state
    showLoginRegisterUI();

}

void ProiectMCQt::showLoginRegisterUI()
{
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    // Clear any existing layout items
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add fields for username and password
    QLabel* usernameLabel = new QLabel("Username:", this);
    QLineEdit* usernameLineEdit = new QLineEdit(this);
    layout->addWidget(usernameLabel, 0, 0);
    layout->addWidget(usernameLineEdit, 0, 1);

    QLabel* passwordLabel = new QLabel("Password:", this);
    QLineEdit* passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Hide password characters
    layout->addWidget(passwordLabel, 1, 0);
    layout->addWidget(passwordLineEdit, 1, 1);

    // Add buttons for register and login
    QPushButton* registerButton = new QPushButton("Register", this);
    QPushButton* loginButton = new QPushButton("Login", this);
    layout->addWidget(registerButton, 2, 0);
    layout->addWidget(loginButton, 2, 1);

    // Connect buttons to their respective functions
    QObject::connect(registerButton, &QPushButton::clicked, [=]() {
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();
        registerUser(username, password);
        });

    QObject::connect(loginButton, &QPushButton::clicked, [=]() {
        QString username = usernameLineEdit->text();
        QString password = passwordLineEdit->text();
        loginUser(username, password);
        });
}

void ProiectMCQt::showUpgradesUI()
{
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    // Add upgrades page elements
    QLabel* upgradesLabel = new QLabel("Upgrades", this);
    QPushButton* upgradeBulletSpeedButton = new QPushButton("Upgrade Bullet Speed", this);
    QPushButton* upgradeFireRateButton = new QPushButton("Upgrade Fire Rate", this);
    QLabel* scoreLabel = new QLabel("Score: 0", this); // Placeholder for actual score
    QLabel* pointsLabel = new QLabel("Points: 0", this); // Placeholder for actual points
    QPushButton* backButton = new QPushButton("Back to Menu", this);

    // Style the title
    upgradesLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = upgradesLabel->font();
    titleFont.setPointSize(16);
    upgradesLabel->setFont(titleFont);

    // Add widgets to the layout
    layout->addWidget(upgradesLabel, 0, 0, 1, 2);
    layout->addWidget(scoreLabel, 1, 0, 1, 2);
    layout->addWidget(pointsLabel, 2, 0, 1, 2);
    layout->addWidget(upgradeBulletSpeedButton, 3, 0, 1, 2);
    layout->addWidget(upgradeFireRateButton, 4, 0, 1, 2);
    layout->addWidget(backButton, 5, 0, 1, 2);

    // Center widgets in the layout
    layout->setAlignment(Qt::AlignCenter);

    // Connect buttons to their respective functions
    connect(upgradeBulletSpeedButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            upgradeBulletSpeed(m_currentUsername);
        }
        });

    connect(upgradeFireRateButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            upgradeFireRate(m_currentUsername);
        }
        });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        showGameMenuUI();
        });

    // Fetch and display current score and points
    if (!m_currentUsername.isEmpty()) {
        fetchScoreAndPoints(m_currentUsername);
    }
}

void ProiectMCQt::updateBullets()
{

}

void ProiectMCQt::registerUser(const QString& username, const QString& password) {
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
            handleNetworkError("Registration", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::loginUser(const QString& username, const QString& password) {
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
            showGameMenuUI();
        }
        else {
            handleNetworkError("Login", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::joinGame(const QString& username) {
    QNetworkRequest request(QUrl(m_baseUrl + "/join_game"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            waitForMatch();
        }
        else {
            handleNetworkError("Join Game", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::checkMatchStatus(const QString& username)
{
}

void ProiectMCQt::sendMoveRequest(const QString& username, const QString& input) {
    QNetworkRequest request(QUrl(m_baseUrl + "/move"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    jsonObj["input"] = input;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            fetchMapData(); // Update the map after movement
        }
        else {
            handleNetworkError("Move", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::sendShootRequest(const QString& username) {
    QNetworkRequest request(QUrl(m_baseUrl + "/shoot"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Handle successful shoot
        }
        else {
            handleNetworkError("Shoot", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::fetchBulletData()
{
    QNetworkRequest request(QUrl(m_baseUrl + "/get_bullet_data"));
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject responseObj = jsonResponse.object();
            // Process bullet data here
        }
        else {
            handleNetworkError("Fetch Bullet Data", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::upgradeBulletSpeed(const QString& username)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/upgrade_bullet_speed"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Handle successful upgrade
        }
        else {
            handleNetworkError("Upgrade Bullet Speed", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::upgradeFireRate(const QString& username)
{
    QNetworkRequest request(QUrl(m_baseUrl + "/upgrade_fire_rate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonObj;
    jsonObj["username"] = username;
    QJsonDocument doc(jsonObj);

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Handle successful upgrade
        }
        else {
            handleNetworkError("Upgrade Fire Rate", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->post(request, doc.toJson());
}

void ProiectMCQt::fetchScoreAndPoints(const QString& username)
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
            // Process score and points data here
        }
        else {
            handleNetworkError("Fetch Score and Points", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::fetchMapData() {
    QNetworkRequest request(QUrl(m_baseUrl + "/map"));

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            if (!doc.isNull()) {
                updateMapDisplay(doc.object());
            }
        }
        else {
            handleNetworkError("Fetch Map", reply->errorString());
        }
        reply->deleteLater();
        sender()->deleteLater();
        });

    manager->get(request);
}

void ProiectMCQt::handleNetworkError(const QString& operation, const QString& error) {
    QMessageBox::critical(this, operation + " Error",
        "An error occurred during " + operation.toLower() + ": " + error);
}

void ProiectMCQt::colorTile(QLabel* label, int type)
{
    QPixmap pixmap;
    QPixmap backgroundPixmap(":/images/Untitled.png");
    switch (type) {
    case 0: // EmptySpace
        label->setAutoFillBackground(false);
        label->clear();
        break;
    case 1: // DestructibleWall
        pixmap = QPixmap(":/images/poza4.png");
        if (pixmap.isNull()) {
            qDebug() << "Failed to load image!";
        }
        else {
            qDebug() << "Image loaded successfully!";
            label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        break;
    case 2: // IndestructibleWall
        //label->setAutoFillBackground(true);
        //label->setPalette(QPalette(Qt::red))
        // ;
        pixmap = QPixmap(":/images/pngwing.com.png");
        label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        break;
    case 3: // DestructibleWallWithBomb
        //label->setAutoFillBackground(true);
        //label->setPalette(QPalette(Qt::green));
        pixmap = QPixmap(":/images/poza4.png");
        label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        break;
    case 4: // Player
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::yellow));
        break;
    case 5: // Bullet
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::white));
        label->setFixedSize(10, 10); // Make bullets smaller than other tiles
        break;
    default:
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::gray));
        break;
    }
}

void ProiectMCQt::showGameMenuUI() {
    // Clear any existing layout items
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    // Add game menu elements
    QLabel* menuLabel = new QLabel("Game Menu", this);
    QPushButton* joinGameButton = new QPushButton("Join Game", this);
    QPushButton* logoutButton = new QPushButton("Logout", this);
    QPushButton* upgradesButton = new QPushButton("Upgrades", this);  // New button for upgrades

    // Style the menu
    menuLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = menuLabel->font();
    titleFont.setPointSize(16);
    menuLabel->setFont(titleFont);

    // Add widgets to layout
    layout->addWidget(menuLabel, 0, 0, 1, 2);
    layout->addWidget(joinGameButton, 1, 0, 1, 2);
    layout->addWidget(upgradesButton, 2, 0, 1, 2); // Add the upgrades button here
    layout->addWidget(logoutButton, 3, 0, 1, 2);

    // Connect buttons
    connect(joinGameButton, &QPushButton::clicked, this, [this]() {
        if (!m_currentUsername.isEmpty()) {
            joinGame(m_currentUsername);
        }
        });

    connect(logoutButton, &QPushButton::clicked, this, [this]() {
        m_currentUsername.clear();
        // Return to login screen
        showLoginRegisterUI();
        });

    connect(upgradesButton, &QPushButton::clicked, this, [this]() {
        showUpgradesUI();  // Show upgrades UI when the button is clicked
        });

    // Center the widgets in the layout
    layout->setAlignment(Qt::AlignCenter);
}

void ProiectMCQt::waitForMatch() {
    // Clear existing layout
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    // Add waiting message
    QLabel* waitingLabel = new QLabel("Waiting for other players...", this);
    waitingLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(waitingLabel, 0, 0);

    // Create and start timer for polling match status
    QTimer* matchTimer = new QTimer(this);
    connect(matchTimer, &QTimer::timeout, this, [this]() {
        QNetworkRequest request(QUrl(m_baseUrl + "/check_match?username=" + m_currentUsername));

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject obj = doc.object();

                if (obj["match_found"].toBool()) {
                    // Stop the timer
                    QTimer* timer = qobject_cast<QTimer*>(sender()->parent());
                    if (timer) {
                        timer->stop();
                        timer->deleteLater();
                    }

                    // Update game state and show game UI
                    fetchMapData();
                }
            }
            reply->deleteLater();
            sender()->deleteLater();
            });

        manager->get(request);
        });

    matchTimer->start(2000); // Check every 2 seconds
}

void ProiectMCQt::updateMapDisplay(const QJsonObject& mapData) {
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());

    // Clear existing layout
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    int height = mapData["height"].toInt();
    int width = mapData["width"].toInt();
    QJsonArray mapArray = mapData["map"].toArray();

    // Create and add tiles to the grid
    for (int i = 0; i < height; ++i) {
        QJsonArray row = mapArray[i].toArray();
        for (int j = 0; j < width; ++j) {
            int tileType = row[j].toInt();
            QLabel* tileLabel = new QLabel(this);
            tileLabel->setFixedSize(30, 30); // Set tile size
            colorTile(tileLabel, tileType);  // Your existing colorTile function
            layout->addWidget(tileLabel, i, j);
        }
    }

    // Add walls from the walls array
    QJsonArray wallsArray = mapData["walls"].toArray();
    for (const QJsonValue& wallValue : wallsArray) {
        QJsonObject wall = wallValue.toObject();
        int x = wall["x"].toInt();
        int y = wall["y"].toInt();
        int type = wall["type"].toInt();

        // Find the existing label at this position and update it
        QLayoutItem* item = layout->itemAtPosition(x, y);
        if (item && item->widget()) {
            QLabel* wallLabel = qobject_cast<QLabel*>(item->widget());
            if (wallLabel) {
                colorTile(wallLabel, type);
            }
        }
    }

    // Make sure the grid cells are evenly spaced
    layout->setSpacing(1);
    layout->setContentsMargins(10, 10, 10, 10);
}