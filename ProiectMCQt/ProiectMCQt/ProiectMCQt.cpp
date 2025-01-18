 #include "ProiectMCQt.h"
#include <cpr/cpr.h>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>
#include <QPixmap>
#include <QDebug>
#include <QBrush>
#include <QGraphicsOpacityEffect>
#include<QLineEdit>
#include<QPushButton>
#include<QTimer>

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

    bulletUpdateTimer = new QTimer(this);
    connect(bulletUpdateTimer, &QTimer::timeout, this, &ProiectMCQt::updateBullets);
    bulletUpdateTimer->start(100);
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
        std::string username = usernameLineEdit->text().toStdString();
        std::string password = passwordLineEdit->text().toStdString();
        registerUser(username, password);
        });

    QObject::connect(loginButton, &QPushButton::clicked, [=]() {
        std::string username = usernameLineEdit->text().toStdString();
        std::string password = passwordLineEdit->text().toStdString();
        loginUser(username, password);
        });
}

void ProiectMCQt::showGameMenuUI()
{
    // Clear any existing layout items
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add a label and buttons for game selection
    QLabel* menuLabel = new QLabel("Choose a game:", this);
    layout->addWidget(menuLabel, 0, 0);

    QPushButton* game1Button = new QPushButton("Game 1", this);
    QPushButton* game2Button = new QPushButton("Game 2", this);
    layout->addWidget(game1Button, 1, 0);
    layout->addWidget(game2Button, 1, 1);

    // Connect buttons to functions that handle game selection
    QObject::connect(game1Button, &QPushButton::clicked, this, &ProiectMCQt::joinGame);
    QObject::connect(game2Button, &QPushButton::clicked, this, &ProiectMCQt::joinGame);
}

void ProiectMCQt::loginUser(const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/login" },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        this->username = username;
        qDebug() << "Login successful!";
        qDebug() << "Saved username:" << QString::fromStdString(this->username);
        // Show game menu after successful login
        showGameMenuUI();
    }
    else {
        qDebug() << "Failed to login: " << QString::fromStdString(response.text);
    }
}


void ProiectMCQt::fetchData()
{
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });
    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject jsonObj = jsonDoc.object();

        size_t height = jsonObj["height"].toInt();
        size_t width = jsonObj["width"].toInt();
        QJsonArray mapArray = jsonObj["map"].toArray();

        QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());

        // Clear existing layout
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

       /* QLabel* backgroundLabel = new QLabel(centralWidget);
        backgroundLabel->setPixmap(QPixmap("C:/Users/Cezar/Desktop/football-pitch.png").scaled(centralWidget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        backgroundLabel->lower();
        layout->addWidget(backgroundLabel, 0, 0, height, width);*/
        QPalette palette;
        QPixmap backgroundPixmap(":/images/Untitled.png");
        palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
        centralWidget->setAutoFillBackground(true);
        centralWidget->setPalette(palette);

        // First, render the base map with bullets
        for (size_t i = 0; i < height; ++i) {
            QJsonArray rowArray = mapArray[i].toArray();
            for (size_t j = 0; j < width; ++j) {
                int tileValue = rowArray[j].toInt();
                QLabel* label = new QLabel();
                label->setFixedSize(20, 20);

                // Check if there's a bullet at this position
                bool hasBullet = false;
                for (const auto& bullet : bullets) {
                    if (bullet.x == i && bullet.y == j) {
                        colorTile(label, 5); // 5 is for bullet
                        hasBullet = true;
                        break;
                    }
                }

                if (!hasBullet) {
                    colorTile(label, tileValue);
                }

                layout->addWidget(label, i, j);
            }
        }

        // Then overlay the walls (this ensures walls are always on top)
        QJsonArray wallsArray = jsonObj["walls"].toArray();
        for (const auto& wallJson : wallsArray) {
            QJsonObject wallObj = wallJson.toObject();
            size_t x = wallObj["x"].toInt();
            size_t y = wallObj["y"].toInt();
            int type = wallObj["type"].toInt();

            // Check if there's a bullet at this position (walls should block bullets)
            bool hasBullet = false;
            for (const auto& bullet : bullets) {
                if (bullet.x == x && bullet.y == y) {
                    hasBullet = true;
                    break;
                }
            }

            // Only render wall if there's no bullet (or you could choose to always render the wall)
            if (!hasBullet) {
                QLabel* label = new QLabel();
                label->setFixedSize(20, 20);
                colorTile(label, type);
                layout->addWidget(label, x, y);
            }
        }
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(response.error.message));
    }
}

void ProiectMCQt::colorTile(QLabel* label, int type)
{
    QPixmap pixmap;
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


void ProiectMCQt::keyPressEvent(QKeyEvent* event)
{
    int x = playerPosition.first;
    int y = playerPosition.second;

    switch (event->key()) {
    case Qt::Key_W:
        x--;
        break;
    case Qt::Key_S:
        x++;
        break;
    case Qt::Key_A:
        y--;
        break;
    case Qt::Key_D:
        y++;
        break;
    case Qt::Key_Space:
        shootBullet(playerID, "up", x, y, 2);
    default:
        return;
    }

    playerPosition = { x, y };

    sendMoveRequest(x, y, playerID);
}

void ProiectMCQt::sendMoveRequest(int x, int y, int playerID)
{
    // Trimite cererea POST către server pentru a muta jucătorul
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/move" },
        cpr::Body{ R"({"x":)" + std::to_string(x) + R"(,"y":)" + std::to_string(y) + R"(,"playerID":)" + std::to_string(playerID) + R"(})" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject jsonObj = jsonDoc.object();

        QJsonArray mapArray = jsonObj["map"].toArray();
        QJsonArray playersArray = jsonObj["players"].toArray();
        for (const auto& player : playersArray) {
            QJsonObject playerObj = player.toObject();
            int id = playerObj["id"].toInt();
            int playerX = playerObj["x"].toInt();
            int playerY = playerObj["y"].toInt();
            // Actualizează harta după mutare
            fetchData();
        }
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(response.error.message));
    }
}


void ProiectMCQt::registerUser(const std::string& username, const std::string& password) {
    //this->username = username;
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/signup" },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        qDebug() << "Account created successfully!";
    }
    else {
        qDebug() << "Failed to create account: " << QString::fromStdString(response.text);
    }
}

void ProiectMCQt::joinGame() {
    auto response = cpr::Post(cpr::Url{ "http://localhost:18080/join_game" },
        cpr::Body{ R"({"username":")" + this->username + R"("})" },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        qDebug() << "Joined game successfully!";
        waitForMatch(); // Wait for players to join the match
    }
    else {
        qDebug() << "Failed to join game: " << QString::fromStdString(response.text);
    }
}

void ProiectMCQt::waitForMatch() {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        auto response = cpr::Get(cpr::Url{ "http://localhost:18080/check_match" },
            cpr::Parameters{ { "username", this->username } });
        if (response.status_code == 200) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
            QJsonObject jsonObj = jsonDoc.object();

            bool matchFound = jsonObj["match_found"].toBool();
            if (matchFound) {
                playerID = jsonObj["player_id"].toInt();
                playerPosition = { jsonObj["x"].toInt(), jsonObj["y"].toInt() };
                fetchData(); // Fetch initial game state
                timer->stop(); // Stop the timer
                showGameUI(); // Show the game UI
            }
        }
        else {
            qDebug() << "Error checking match status: " << QString::fromStdString(response.error.message);
        }
        });
    timer->start(5000); // Check every 5 seconds
}

void ProiectMCQt::showGameUI()
{
    // Clear any existing layout items
    QGridLayout* layout = qobject_cast<QGridLayout*>(centralWidget->layout());
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Fetch and display the map data
    fetchData();
}

void ProiectMCQt::shootBullet(int playerID, const std::string& direction, int x, int y, int speed)
{
    // Create the JSON body for the request
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/shoot" },
        cpr::Body{ R"({"playerID":)" + std::to_string(playerID) + R"(,"direction":")" + direction + R"(","x":)" + std::to_string(x) + R"(,"y":)" + std::to_string(y) + R"(,"speed":)" + std::to_string(speed) + R"(})" },
        cpr::Header{ {"Content-Type", "application/json"} });

    // Check if the shooting was successful
    if (response.status_code == 200) {
        qDebug() << "Bullet fired successfully!";
    }
    else {
        qDebug() << "Failed to fire bullet: " << QString::fromStdString(response.text);
    }
}

void ProiectMCQt::updateBullets()
{
    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/bullets" });
    if (response.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray bulletsArray = jsonDoc.array();

        bullets.clear();
        for (const auto& bulletJson : bulletsArray) {
            QJsonObject bulletObj = bulletJson.toObject();
            Bullet bullet{
                bulletObj["x"].toInt(),
                bulletObj["y"].toInt(),
                bulletObj["direction"].toString().toStdString(),
                bulletObj["playerID"].toInt()
            };
            bullets.push_back(bullet);
        }
        fetchData(); // Refresh the map display
    }
}