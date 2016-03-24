#include "eventdialog.h"
#include "ui_eventdialog.h"

EventDialog::EventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    this->setFixedWidth(400);
    this->setFixedHeight(500);
    this->setModal(true);
    this->setWindowTitle("Event Manager");

    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *first_row = new QHBoxLayout;
    QLabel *label_name = new QLabel("Name: ");
    QLineEdit *edit_name = new QLineEdit;
    first_row->addWidget(label_name);
    first_row->addWidget(edit_name);
    main_layout->addLayout(first_row);
    main_layout->addWidget(new QLabel("Description: "));
    main_layout->addWidget(new QTextEdit);
    QHBoxLayout *second_row = new QHBoxLayout;
    QLabel *label_category = new QLabel("Category: ");
    QLineEdit *edit_category = new QLineEdit;
    second_row->addWidget(label_category);
    second_row->addWidget(edit_category);
    main_layout->addLayout(second_row);
    QHBoxLayout *third_row = new QHBoxLayout;
    QLabel *label_start = new QLabel("Start: ");
    QLineEdit *edit_start = new QLineEdit;
    third_row->addWidget(label_start);
    third_row->addWidget(edit_start);
    main_layout->addLayout(third_row);
    QHBoxLayout *fourth_row = new QHBoxLayout;
    QLabel *label_end = new QLabel("End: ");
    QLineEdit *edit_end = new QLineEdit;
    fourth_row->addWidget(label_end);
    fourth_row->addWidget(edit_end);
    main_layout->addLayout(fourth_row);
    QHBoxLayout *fifth_row = new QHBoxLayout;
    QPushButton *button_cancel = new QPushButton("Cancel");
    QPushButton *button_save = new QPushButton("Save");
    fifth_row->addWidget(button_cancel);
    fifth_row->addWidget(button_save);
    main_layout->addLayout(fifth_row);

    this->setLayout(main_layout);
}

EventDialog::~EventDialog()
{
    delete ui;
}
