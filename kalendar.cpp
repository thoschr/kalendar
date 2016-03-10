#include "kalendar.h"
#include "ui_kalendar.h"

Kalendar::Kalendar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kalendar)
{
    ui->setupUi(this);
}

Kalendar::~Kalendar()
{
    delete ui;
}
