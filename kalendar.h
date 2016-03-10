#ifndef KALENDAR_H
#define KALENDAR_H

#include <QMainWindow>

namespace Ui {
class Kalendar;
}

class Kalendar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kalendar(QWidget *parent = 0);
    ~Kalendar();

private:
    Ui::Kalendar *ui;
};

#endif // KALENDAR_H
