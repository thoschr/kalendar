#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QListIterator>
#include <QColor>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include "ui_monthview.h"
#include "eventdialog.h"
#include "qframe_extended.h"
#include "qwidget_extended.h"
#include "categorydialog.h"
#include "view.h"
#include "util/dateutil.h"
#include "qlabel_event.h"
#include "qpushbutton_extended.h"
#include "daydialog.h"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow, public View
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
    bool is_color_dark(string colorName);
    void remove_events_from_all_frames();
    void remove_events_from_frame(int i);
    QLabelEvent *createLabelEvent(Event *event);
    void createMenu();
    void on_button_extended_click(int index);

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

public:
    explicit MonthView(QWidget *parent = 0);
    virtual void display_events(Date date);
    virtual void display_events(Date date, Category category);
    ~MonthView();
public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);
    void on_mouse_move(QFrameExtended *frame);
    void on_back_button_click();
    void on_next_button_click();
    void on_event_click(Event *event);

private slots:
    void exportEvents();
    void importEvents();
    void addEvent();
    void editCategories();
};

#endif // MONTHVIEW_H
