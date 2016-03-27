#include "eventdialog.h"
#include "ui_eventdialog.h"

#include <QDebug>


EventDialog::EventDialog(Date start_date, Date end_date, string name, string description, string categoryName, QWidget *parent) :
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
    this->edit_name = new QLineEdit(name.c_str());
    first_row->addWidget(label_name);
    first_row->addWidget(this->edit_name);
    main_layout->addLayout(first_row);
    main_layout->addWidget(new QLabel("Description: "));
    this->edit_description = new QPlainTextEdit(description.c_str());
    main_layout->addWidget(this->edit_description);
    QHBoxLayout *second_row = new QHBoxLayout;
    QLabel *label_category = new QLabel("Category: ");
    this->edit_category = new QComboBox;
    this->category_list = this->pm->get_categories();
    int index = 0;
    for (Category *category : category_list) {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->edit_category->addItem(QIcon(pixmap), QString(category->getName().c_str()));
        if (category->getName() == categoryName)
            this->edit_category->setCurrentIndex(index);
        index++;
    }
    second_row->addWidget(label_category);
    second_row->addWidget(this->edit_category);
    main_layout->addLayout(second_row);
    QHBoxLayout *third_row = new QHBoxLayout;
    QLabel *label_start = new QLabel("Start: ");
    this->edit_start = new QDateTimeEdit;
    this->edit_start->setCalendarPopup(true);
    this->edit_start->setDateTime(QDateTime(QDate(start_date.getYear(), start_date.getMonth(), start_date.getMonthDay())));
    third_row->addWidget(label_start);
    third_row->addWidget(this->edit_start);
    main_layout->addLayout(third_row);
    QHBoxLayout *fourth_row = new QHBoxLayout;
    QLabel *label_end = new QLabel("End: ");
    this->edit_end = new QDateTimeEdit;
    this->edit_end->setCalendarPopup(true);
    this->edit_end->setDateTime(QDateTime(QDate(end_date.getYear(), end_date.getMonth(), end_date.getMonthDay())));
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
    delete this;
}

void EventDialog::on_button_save_click() {
    QMessageBox msg;
    msg.setWindowTitle("Error");
    msg.setIconPixmap(QIcon::fromTheme("error").pixmap(40,40));
    if (this->edit_name->text().length() < 3) {
        msg.setText("The name must have a length greater than 2.");
        msg.exec();
        return;
    }
    Category *category;
    for (Category *c : this->category_list) {
        if (this->edit_category->currentText().toStdString() == c->getName()) {
            category = c;
            break;
        }
    }

    Event newEvent(0, this->edit_name->text().toStdString(), this->edit_description->toPlainText().toStdString(), category, this->edit_start->dateTime().toTime_t(), this->edit_end->dateTime().toTime_t());
    if (this->pm->add_event(&newEvent)) {
        this->close();
        delete this;
    } else {
        msg.setText("Persistence error");
        msg.exec();
    }

}
