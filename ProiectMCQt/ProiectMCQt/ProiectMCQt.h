#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProiectMCQt.h"

class ProiectMCQt : public QMainWindow
{
    Q_OBJECT

public:
    ProiectMCQt(QWidget *parent = nullptr);
    ~ProiectMCQt();

private:
    Ui::ProiectMCQtClass ui;
};
