#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "RegisterWidget.h"
#include "LoginWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget mainWidget;
    QVBoxLayout mainLayout(&mainWidget);

    RegisterWidget registerWidget;
    LoginWidget loginWidget;

    mainLayout.addWidget(&registerWidget);
    mainLayout.addWidget(&loginWidget);

    mainWidget.setLayout(&mainLayout);
    mainWidget.show();

    return app.exec();
}
