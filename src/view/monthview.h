#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QListIterator>
#include <QColor>
#include <QtAlgorithms>
#include "model/event.h"
#include "model/category.h"
#include "util/dateutil.h"
#include "qframe_extended.h"
#include "qlabel_event.h"

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
    void display_events(Date date);
    void display_events(Date date, Category category);
    bool is_color_dark(string colorName);

public:
    explicit MonthView(QWidget *parent = 0);
    ~MonthView();
public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);
    void on_mouse_move(QFrameExtended *frame);
    void on_back_button_click();
    void on_next_button_click();
    void on_event_click(Event *event);
};

#endif // MONTHVIEW_H
