#include "LoginWidget.h"
#undef signals
#include <crow.h>
#define signals Q_SIGNALS
#include <cpr/cpr.h>

LoginWidget::LoginWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* usernameLabel = new QLabel("Username:", this);
    usernameLineEdit = new QLineEdit(this);
    QPushButton* loginButton = new QPushButton("Login", this);

    layout->addWidget(usernameLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
}

void LoginWidget::onLoginClicked() {
    std::string username = usernameLineEdit->text().toStdString();

    crow::json::wvalue jsonData;
    jsonData["username"] = username;

    auto response = cpr::Post(cpr::Url{ "http://localhost:18080/login" },
        cpr::Body{ jsonData.dump() },
        cpr::Header{ {"Content-Type", "application/json"} });

    if (response.status_code == 200) {
        QLabel* successLabel = new QLabel("Login successful", this);
        successLabel->setStyleSheet("color: green;");
        layout()->addWidget(successLabel);
    }
    else {
        QLabel* errorLabel = new QLabel(QString::fromStdString("Error: " + response.text), this);
        errorLabel->setStyleSheet("color: red;");
        layout()->addWidget(errorLabel);
    }
}
