#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "RegisterWidget.h"
#include "LoginWidget.h"
#include"ProiectMCQt.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);


    ProiectMCQt window; 
    window.show();
    /*QWidget mainWidget;
    QVBoxLayout mainLayout(&mainWidget);

    RegisterWidget registerWidget;
    LoginWidget loginWidget;

    mainLayout.addWidget(&registerWidget);
    mainLayout.addWidget(&loginWidget);

    mainWidget.setLayout(&mainLayout);
    mainWidget.show();*/

    return app.exec();
}
