#include "categoryselectdialog.h"

#include <QDebug>

CategorySelectDialog::CategorySelectDialog(View *parentView, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectDialog)
{
    this->parent = parentView;
    this->setWindowTitle("Category Selector");
    this->pm = new PManager;
    this->selected_category = 1;
    QHBoxLayout *button_layout = new QHBoxLayout;
    QVBoxLayout *main_layout = new QVBoxLayout;
    this->list_categories = new QComboBox;
    load_categories();
    QPushButton *button_ok = new QPushButton("OK");
    QPushButton *button_cancel = new QPushButton("Cancel");
    button_cancel->setFixedWidth(50);
    button_ok->setFixedWidth(50);
    connect(button_cancel, &QPushButton::clicked, this, &CategorySelectDialog::on_button_cancel_click);
    connect(button_ok, &QPushButton::clicked, this, &CategorySelectDialog::on_button_ok_click);
    QLabel *message = new QLabel("Select a category for the imported events: ");
    main_layout->addWidget(message);
    main_layout->addWidget(this->list_categories);
    button_layout->addWidget(button_ok);
    button_layout->addWidget(button_cancel);
    main_layout->addLayout(button_layout);
    this->setLayout(main_layout);
}

void CategorySelectDialog::load_categories() {
    for (Category *category : this->category_list) delete category;
    this->category_list = this->pm->get_categories();
    for (Category *category : category_list) {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->list_categories->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
}

void CategorySelectDialog::on_button_cancel_click() {
    this->selected_category = 0; /* Invalid category id */
    this->close();
    delete this;
}

void CategorySelectDialog::on_button_ok_click() {
    for (Category *c : this->category_list) {
        if (this->list_categories->currentText().toStdString() == c->getName()) {
            this->selected_category = c->getId();
            break;
        }
    }
    this->close();
    delete this;
}

unsigned int CategorySelectDialog::getSelectedCategory() {
    return this->selected_category;
}

CategorySelectDialog::~CategorySelectDialog()
{
    delete ui;
    delete pm;
    for (Category *category : this->category_list) delete category;
}
