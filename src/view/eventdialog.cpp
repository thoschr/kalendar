#include "eventdialog.h"
#include "ui_eventdialog.h"

#include <QDebug>


EventDialog::EventDialog(Date *start_date, Date *end_date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    this->setFixedWidth(400);
    this->setFixedHeight(500);
    this->setModal(true);
    this->setWindowTitle("Event Manager");

    this->pm = new PManager;

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *first_row = new QHBoxLayout;
    QLabel *label_name = new QLabel("Name: ");
    this->edit_name = new QLineEdit;
    first_row->addWidget(label_name);
    first_row->addWidget(this->edit_name);
    main_layout->addLayout(first_row);
    main_layout->addWidget(new QLabel("Description: "));
    this->edit_description = new QPlainTextEdit;
    main_layout->addWidget(this->edit_description);
    QHBoxLayout *second_row = new QHBoxLayout;
    QLabel *label_category = new QLabel("Category: ");
    this->edit_category = new QComboBox;
    list<Category*> list_categories = this->pm->get_categories();
    for (Category *category : list_categories) {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->edit_category->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
    second_row->addWidget(label_category);
    second_row->addWidget(this->edit_category);
    main_layout->addLayout(second_row);
    QHBoxLayout *third_row = new QHBoxLayout;
    QLabel *label_start = new QLabel("Start: ");
    this->edit_start = new QDateTimeEdit;
    this->edit_start->setCalendarPopup(true);
    this->edit_start->setDateTime(QDateTime(QDate(start_date->getYear(), start_date->getMonth(), start_date->getMonthDay())));
    third_row->addWidget(label_start);
    third_row->addWidget(this->edit_start);
    main_layout->addLayout(third_row);
    QHBoxLayout *fourth_row = new QHBoxLayout;
    QLabel *label_end = new QLabel("End: ");
    this->edit_end = new QDateTimeEdit;
    this->edit_end->setCalendarPopup(true);
    this->edit_end->setDateTime(QDateTime(QDate(end_date->getYear(), end_date->getMonth(), end_date->getMonthDay())));
    fourth_row->addWidget(label_end);
    fourth_row->addWidget(this->edit_end);
    main_layout->addLayout(fourth_row);
    QHBoxLayout *fifth_row = new QHBoxLayout;
    QPushButton *button_cancel = new QPushButton("Cancel");
    connect(button_cancel, &QPushButton::clicked, this, &EventDialog::on_button_cancel_click);
    QPushButton *button_save = new QPushButton("Save");
    connect(button_save, &QPushButton::clicked, this, &EventDialog::on_button_save_click);
    fifth_row->addWidget(button_cancel);
    fifth_row->addWidget(button_save);
    main_layout->addLayout(fifth_row);

    this->setLayout(main_layout);
}

EventDialog::~EventDialog()
{
    delete ui;
    delete this->pm;
}

void EventDialog::on_button_cancel_click() {
    this->close();
}

void EventDialog::on_button_save_click() {
    if (this->edit_name->text().length() < 3) {
        QMessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText("The name must have a length greater than 2.");
        msg.setIconPixmap(QIcon::fromTheme("error").pixmap(40,40));
        msg.exec();
        return;
    }
    Event newEvent(0, this->edit_name->text().toStdString(), this->edit_description->toPlainText().toStdString(), this->edit_category->currentText().toStdString(), this->edit_start->dateTime().toTime_t(), this->edit_end->dateTime().toTime_t());
    this->pm->add_event(&newEvent);
    this->close();
}
