#include "RegisterWidget.h"
#undef signals
#include <crow.h>
#define signals Q_SIGNALS
#include <cpr/cpr.h>

RegisterWidget::RegisterWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* usernameLabel = new QLabel("Username:", this);
    usernameLineEdit = new QLineEdit(this);

    QLabel* passwordLabel = new QLabel("Password:", this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton* registerButton = new QPushButton("Register", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(registerButton);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterClicked);
}

void RegisterWidget::onRegisterClicked() {

    std::string username = usernameLineEdit->text().toStdString();
    std::string password = passwordLineEdit->text().toStdString();

    crow::json::wvalue jsonData;
    jsonData["username"] = username;
    jsonData["password"] = password;

    // Send POST request to server for signup
    auto response = cpr::Post(cpr::Url{ "http://localhost:18080/signup" },
        cpr::Body{ jsonData.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });

    QVBoxLayout* currentLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!currentLayout) return;

    // Update UI based on the response
    if (response.status_code == 200) {
        QLabel* successLabel = new QLabel("User registered successfully", this);
        successLabel->setStyleSheet("color: green;");
        currentLayout->addWidget(successLabel);
    }
    else {
        QLabel* errorLabel = new QLabel(QString::fromStdString("Error: " + response.text), this);
        errorLabel->setStyleSheet("color: red;");
        currentLayout->addWidget(errorLabel);
    }
}