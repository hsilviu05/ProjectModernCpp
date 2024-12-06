#include <QApplication>
#include "client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Creează o instanță a clasei Client
    Client client;

    // Setează imaginea de fundal pentru terenul de fotbal
    //client.setupMapBackground(":/resources/football-pitch.png");

    // Afișează fereastra principală
    client.show();

    return app.exec();
}
