#include "ProiectMCQt.h"
#include <cpr/cpr.h>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>

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
                QLabel* label = new QLabel(QString::number(tileValue));
                label->setAlignment(Qt::AlignCenter);
                layout->addWidget(label, i, j);
            }
        }

        QJsonArray wallsArray = jsonObj["walls"].toArray();
        for (const auto& wallJson : wallsArray) {
            QJsonObject wallObj = wallJson.toObject();
            size_t x = wallObj["x"].toInt();
            size_t y = wallObj["y"].toInt();
            QLabel* label = new QLabel("W");
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label, x, y);
        }
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(response.error.message));
    }
}
