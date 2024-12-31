#ifndef PROIECTMCQT_H
#define PROIECTMCQT_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QLabel>

class ProiectMCQt : public QMainWindow
{
    Q_OBJECT

public:
    ProiectMCQt(QWidget* parent = nullptr);
    ~ProiectMCQt();

private slots:
    void fetchData();
    void colorWall(QLabel* label, int type);
    void colorTile(QLabel* label, int type);


private:
    void setupUI(); // Noua metodă pentru configurarea UI-ului

    QWidget* centralWidget;
};

#endif // PROIECTMCQT_H
