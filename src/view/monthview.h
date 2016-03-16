#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonthView(QWidget *parent = 0);
    ~MonthView();

private:
    Ui::MonthView *ui;
};

#endif // MONTHVIEW_H
