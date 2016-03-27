#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QComboBox>
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

public:
    explicit EventDialog(Date *start_date, Date *end_date, QWidget *parent = 0);
    ~EventDialog();

public slots:
    void on_button_cancel_click();
    void on_button_save_click();
};

#endif // EVENTDIALOG_H
