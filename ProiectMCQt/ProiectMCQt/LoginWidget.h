#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    LoginWidget(QWidget* parent = nullptr);

private slots:
    void onLoginClicked();

private:
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
};

#endif // LOGINWIDGET_H
