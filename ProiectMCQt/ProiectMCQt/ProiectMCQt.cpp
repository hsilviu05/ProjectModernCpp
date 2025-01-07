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

ProiectMCQt::ProiectMCQt(QWidget* parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_TranslucentBackground); // Permite transparența pentru fereastra principală
    setupUI();
    fetchData();
}

ProiectMCQt::~ProiectMCQt()
{
    // Curățarea resurselor, dacă este necesar
}

//void ProiectMCQt::setupUI()
//{
//    centralWidget = new QWidget(this);
//    setCentralWidget(centralWidget);
//
//    QGridLayout* layout = new QGridLayout(centralWidget);
//    centralWidget->setLayout(layout);
//
//    // Setează imaginea de fundal
//    QPixmap background("C:/Users/Cezar/Desktop/football-pitch.png");
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(background));
//    centralWidget->setAutoFillBackground(true);
//    centralWidget->setPalette(palette);
//}


void ProiectMCQt::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QGridLayout* layout = new QGridLayout(centralWidget);
    centralWidget->setLayout(layout);

    // Adaugă câmpuri pentru username și password
    QLabel* usernameLabel = new QLabel("Username:", this);
    QLineEdit* usernameLineEdit = new QLineEdit(this);
    layout->addWidget(usernameLabel, 0, 0);
    layout->addWidget(usernameLineEdit, 0, 1);

    QLabel* passwordLabel = new QLabel("Password:", this);
    QLineEdit* passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password); // Setați câmpul parolei să ascundă caracterele
    layout->addWidget(passwordLabel, 1, 0);
    layout->addWidget(passwordLineEdit, 1, 1);

    // Adaugă butoanele pentru register și login
    QPushButton* registerButton = new QPushButton("Register", this);
    QPushButton* loginButton = new QPushButton("Login", this);
    layout->addWidget(registerButton, 2, 0);
    layout->addWidget(loginButton, 2, 1);

    // Conectează butoanele la funcțiile respective
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

        for (size_t i = 0; i < height; ++i) {
            QJsonArray rowArray = mapArray[i].toArray();
            for (size_t j = 0; j < width; ++j) {
                int tileValue = rowArray[j].toInt();
                QLabel* label = new QLabel();
                label->setFixedSize(20, 20); // Setați dimensiunea pătratelor
                colorTile(label, tileValue);
                layout->addWidget(label, i, j);
            }
        }

        QJsonArray wallsArray = jsonObj["walls"].toArray();
        for (const auto& wallJson : wallsArray) {
            QJsonObject wallObj = wallJson.toObject();
            size_t x = wallObj["x"].toInt();
            size_t y = wallObj["y"].toInt();
            int type = wallObj["type"].toInt();
            QLabel* label = new QLabel();
            label->setFixedSize(20, 20); // Setați dimensiunea pătratelor
            colorTile(label, type);
            layout->addWidget(label, x, y);
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
        pixmap = QPixmap("C:/Users/Cezar/Desktop/pngwing.com.png");
        if (pixmap.isNull()) {
            qDebug() << "Failed to load image!";
        }
        else {
            qDebug() << "Image loaded successfully!";
            label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        break;
    case 2: // IndestructibleWall
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::red));
        break;
    case 3: // DestructibleWallWithBomb
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::green));
        break;
    case 4: // Player
        label->setAutoFillBackground(true);
        label->setPalette(QPalette(Qt::yellow));
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
    default:
        return;
    }
    sendMoveRequest(x, y, playerID);
}

void ProiectMCQt::sendMoveRequest(int x, int y, int playerID)
{
    // Trimite cererea POST către server pentru a muta jucătorul
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/move" },
        cpr::Body{ R"({"x":)" + std::to_string(x) + R"(,"y":)" + std::to_string(y) + R"(,"playerID":)" + std::to_string(playerID) + R"(})" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code == 200) {
        // Actualizează harta după mutare
        fetchData();
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(response.error.message));
    }
}


void ProiectMCQt::registerUser(const std::string& username, const std::string& password) {
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

void ProiectMCQt::loginUser(const std::string& username, const std::string& password) {
    cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/login" },
        cpr::Body{ R"({"username":")" + username + R"(","password":")" + password + R"("})" },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        qDebug() << "Login successful!";
    }
    else {
        qDebug() << "Failed to login: " << QString::fromStdString(response.text);
    }
}
