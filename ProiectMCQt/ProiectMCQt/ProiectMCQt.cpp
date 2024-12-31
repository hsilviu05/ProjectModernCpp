#include "ProiectMCQt.h"
#include <cpr/cpr.h>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
