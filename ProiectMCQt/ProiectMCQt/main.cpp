#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include"ProiectMCQt.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);


    ProiectMCQt window; 
    window.show();
  
    return app.exec();
}
