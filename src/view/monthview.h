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
#include <QDir>
#include <QFileDialog>
#include <QScrollArea>
#include "ui_monthview.h"
#include "eventdialog.h"
#include "qframe_extended.h"
#include "qwidget_extended.h"
#include "categorydialog.h"
#include "view.h"
#include "util/dateutil.h"
#include "qlabel_event.h"
#include "qpushbutton_extended.h"
#include "customdialog.h"
#include "categoryselectdialog.h"

/* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted.
 * This should be used when you don't care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

#define MAINWINDOW_STYLE "#mainwindow { background-color: #FFFFFF; border: 2px solid #DDDDFF; }"

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; color: #FF0000; }" \
                   "QFrame#selected { background-color: #EEEEFF; }" \
                   "QLabel#header { font-weight: bold; }" \
                   "QFrame#header { background-color: #DDDDFF; }"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow, public View
{
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QLabel *label_date;
    QPushButton *todobutton;
    Date *selection_start;
    Date *selection_end;
    Ui::MonthView *ui;
    PManager *pm;
    QFrameExtended *frames[42]; //I have a 7x7 grid, but without consider the first row I've a total of 6x7 cells
    void display_days(Date date);
    bool is_color_dark(string colorName);
    void remove_events_from_all_frames();
    void remove_events_from_frame(int i);
    QLabelEvent *createLabelEvent(Event *event);
    QFrameExtended *createQFrameExtended(Date *date);
    void createMenu();
    void on_button_extended_click(int index);

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

public:
    explicit MonthView(QWidget *parent = 0);
    void refresh_events();
    void refresh_todos();
    void display_events(Date date);
    void display_events(Date date, Category category);
    ~MonthView();

public slots:
    void on_mouse_press(QFrameExtended *frame);
    void on_mouse_release(QFrameExtended *frame);
    void on_mouse_move(QFrameExtended *frame);
    void on_back_button_click();
    void on_next_button_click();
    void on_event_click(QLabelEvent *label_event, Qt::MouseButton button);
    void on_todo_button_click();

private slots:
    void save_events();
    void load_events();
    void import_events();
    void add_event();
    void edit_categories();
    void show_agenda(bool only_todos = false);
};

#endif // MONTHVIEW_H
