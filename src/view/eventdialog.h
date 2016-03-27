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
#include "model/date.h"
#include "model/category.h"
#include "model/event.h"
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
    PManager *pm;
    list<Category *> category_list;

public:
    explicit EventDialog(Date start_date, Date end_date, string name = "", string description = "", string categoryName = "", QWidget *parent = 0);
    ~EventDialog();

public slots:
    void on_button_cancel_click();
    void on_button_save_click();
};

#endif // EVENTDIALOG_H
