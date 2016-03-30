#ifndef QPUSHBUTTONEXTENDED_H
#define QPUSHBUTTONEXTENDED_H

#include <QPushButton>

class QPushButtonExtended : public QPushButton
{
    Q_OBJECT
private:
    int index;

public:
    explicit QPushButtonExtended(const char *text, QWidget *parent = 0);
    void setIndex(int index);
    void button_clicked();

signals:
    void on_click(int index);

public slots:
};

#endif // QPUSHBUTTONEXTENDED_H
