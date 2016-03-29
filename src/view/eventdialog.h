#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QIconEngine>
#include <QPixmap>
#include <QIcon>
#include "util/dateutil.h"
#include "view.h"
#include "persistence/pmanager.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::EventDialog *ui;
    QLineEdit *edit_name;
    QComboBox *edit_category;
    QDateTimeEdit *edit_start;
    QDateTimeEdit *edit_end;
    QPlainTextEdit *edit_description;
    QPushButton *button_delete;
    View *parent;
    PManager *pm;
    Event *event;
    list<Category *> category_list;
    void refresh(Date date);

public:
    explicit EventDialog(View *parentView, Date start_date = Date(), Date end_date = Date(), QWidget *parent = 0);
    ~EventDialog();
    void setEvent(Event *event);

public slots:
    void on_button_cancel_click();
    void on_button_save_click();
    void on_button_delete_click();
};

#endif // EVENTDIALOG_H
