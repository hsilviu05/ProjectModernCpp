#include "RegisterWidget.h"
#include <cpr/cpr.h>
#include <json/json.h>

RegisterWidget::RegisterWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* usernameLabel = new QLabel("Username:", this);
    QLabel* nameLabel = new QLabel("Name:", this);
    usernameLineEdit = new QLineEdit(this);
    nameLineEdit = new QLineEdit(this);
    QPushButton* registerButton = new QPushButton("Register", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterClicked);
}

void RegisterWidget::onRegisterClicked() {
    std::string username = usernameLineEdit->text().toStdString();
    std::string name = nameLineEdit->text().toStdString();

    Json::Value jsonData;
    jsonData["username"] = username;
    jsonData["name"] = name;

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, jsonData);

    auto response = cpr::Post(cpr::Url{ "http://localhost:18080/register" },
        cpr::Body{ jsonString },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        QLabel* successLabel = new QLabel("User registered successfully", this);
        successLabel->setStyleSheet("color: green;");
        layout()->addWidget(successLabel);
    }
    else {
        QLabel* errorLabel = new QLabel(QString::fromStdString("Error: " + response.text), this);
        errorLabel->setStyleSheet("color: red;");
        layout()->addWidget(errorLabel);
    }
}
