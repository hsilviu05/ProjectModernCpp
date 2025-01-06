#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class RegisterWidget : public QWidget {
    Q_OBJECT

public:
    RegisterWidget(QWidget* parent = nullptr);

private slots:
    void onRegisterClicked();

private:
    QLineEdit* usernameLineEdit;
    QLineEdit* passwordLineEdit;
};

#endif
