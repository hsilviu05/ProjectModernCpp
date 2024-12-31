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

ProiectMCQt::ProiectMCQt(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    fetchData();
}

ProiectMCQt::~ProiectMCQt()
{
    // Curățarea resurselor, dacă este necesar
}

void ProiectMCQt::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QGridLayout* layout = new QGridLayout(centralWidget);
    centralWidget->setLayout(layout);
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
            colorWall(label, type);
            layout->addWidget(label, x, y);
        }
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(response.error.message));
    }
}

void ProiectMCQt::colorTile(QLabel* label, int type)
{
    QPalette palette = label->palette();
    switch (type) {
    case 0: // EmptySpace
        palette.setColor(QPalette::Window, Qt::white);
        break;
    case 1: // DestrucitbleWall
        palette.setColor(QPalette::Window, Qt::blue);
        break;
    case 2: // IndestrucitbleWall
        palette.setColor(QPalette::Window, Qt::red);
        break;
    case 3: // DestrucitbleWallWithBomb
        palette.setColor(QPalette::Window, Qt::green);
        break;
    case 4: // Player
        palette.setColor(QPalette::Window, Qt::yellow);
        break;
    default:
        palette.setColor(QPalette::Window, Qt::gray);
        break;
    }
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}

void ProiectMCQt::colorWall(QLabel* label, int type)
{
    QPalette palette = label->palette();
    switch (type) {
    case 1: // DestrucitbleWall
        palette.setColor(QPalette::Window, Qt::blue);
        break;
    case 2: // IndestrucitbleWall
        palette.setColor(QPalette::Window, Qt::red);
        break;
    case 3: // DestrucitbleWallWithBomb
        palette.setColor(QPalette::Window, Qt::green);
        break;
    default:
        palette.setColor(QPalette::Window, Qt::black);
        break;
    }
    label->setAutoFillBackground(true);
    label->setPalette(palette);
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

