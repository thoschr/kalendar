#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include "util/dateutil.h"
#include "qframe_extended.h"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow
{
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QLabel *label_date;
    Date *selection_start;
    Date *selection_end;
    Ui::MonthView *ui;
    QFrameExtended *frames[42]; //I have a 7x7 grid, but without consider the first row I've a total of 6x7 cells
    void display_days(Date date);

public:
    explicit MonthView(QWidget *parent = 0);
    ~MonthView();
public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);
    void on_back_button_click();
    void on_next_button_click();
};

#endif // MONTHVIEW_H
