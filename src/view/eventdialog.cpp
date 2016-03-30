#include "eventdialog.h"
#include "ui_eventdialog.h"

#include <QDebug>

void EventDialog::setEvent(Event *event) {
    this->event = event;
    if (event != NULL) {
        this->edit_name->setText(event->getName().c_str());
        this->edit_description->setPlainText(event->getDescription().c_str());
        int index = 0;
        for (Category *category : category_list) {
            if (category->getName() == event->getCategory()->getName())
                this->edit_category->setCurrentIndex(index);
            index++;
        }
        Date start_date = DateUtil::date_from_timestamp(event->getStart());
        Date end_date = DateUtil::date_from_timestamp(event->getEnd());
        this->edit_start->setDateTime(QDateTime(QDate(start_date.getYear(), start_date.getMonth(), start_date.getMonthDay())));
        this->edit_end->setDateTime(QDateTime(QDate(end_date.getYear(), end_date.getMonth(), end_date.getMonthDay())));
        this->button_delete->setEnabled(true);
    }
}

EventDialog::EventDialog(View *parentView, Date start_date, Date end_date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    this->parent = parentView;
    this->setFixedWidth(400);
    this->setFixedHeight(500);
    this->setModal(true);
    this->setWindowTitle("Event Manager");

    this->pm = new PManager;
    this->event = NULL;

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
    this->category_list = this->pm->get_categories();
    for (Category *category : category_list) {
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
    button_delete = new QPushButton("Delete");
    button_delete->setEnabled(false);
    connect(button_delete, &QPushButton::clicked, this, &EventDialog::on_button_delete_click);
    QPushButton *button_save = new QPushButton("Save");
    connect(button_save, &QPushButton::clicked, this, &EventDialog::on_button_save_click);
    fifth_row->addWidget(button_cancel);
    fifth_row->addWidget(button_delete);
    fifth_row->addWidget(button_save);
    main_layout->addLayout(fifth_row);

    this->setLayout(main_layout);
}

EventDialog::~EventDialog()
{
    delete ui;
    delete this->pm;
    for (Category *c : this->category_list) delete c;
}

void EventDialog::on_button_cancel_click() {
    this->close();
    delete this;
}

void EventDialog::on_button_delete_click() {
    this->pm->remove_event(this->event);
    refresh(DateUtil::date_from_timestamp(this->event->getStart()));
}

void EventDialog::refresh(Date date) {
    this->parent->display_events(date);
    this->close();
    delete this;
}

//if the event already exists, this function updates it, otherwise will be created
void EventDialog::on_button_save_click() {
    /* otherwise I create a new Event */
    if (this->edit_name->text().length() < 3) {
        QMessageBox::critical(this, "Error", "The name must have a length greater than 2", QMessageBox::Ok);
        return;
    }
    Category *category;
    for (Category *c : this->category_list) {
        if (this->edit_category->currentText().toStdString() == c->getName()) {
            category = new Category(*c);
            break;
        }
    }

    Event newEvent(0, this->edit_name->text().toStdString(), this->edit_description->toPlainText().toStdString(), category, this->edit_start->dateTime().toTime_t(), this->edit_end->dateTime().toTime_t());

    /* If the users has changed an existent event, I'll call the right function */
    if ((this->event != NULL) && (this->pm->edit_event(this->event, &newEvent))) {
        refresh(DateUtil::date_from_timestamp(newEvent.getStart()));
    } else if ((this->event == NULL) && (this->pm->add_event(&newEvent))) { //else I'll create a new Event
        refresh(DateUtil::date_from_timestamp(newEvent.getStart()));
    } else {
        QMessageBox::critical(this, "Error", "Persistence error", QMessageBox::Ok);
    }
}
