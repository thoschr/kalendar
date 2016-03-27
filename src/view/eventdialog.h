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

public:
    explicit EventDialog(Date *start_date, Date *end_date, QWidget *parent = 0);
    ~EventDialog();

private:
    Ui::EventDialog *ui;
};

#endif // EVENTDIALOG_H
