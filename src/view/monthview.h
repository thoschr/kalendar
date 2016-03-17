#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include "util/timeutil.h"
#include "qframe_extended.h"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonthView(QWidget *parent = 0);
    ~MonthView();
public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);

private:
    QGridLayout *grid_layout;
    Time selection_start;
    Time selection_end;
    Ui::MonthView *ui;
    void display_days(Time time);
};

#endif // MONTHVIEW_H
