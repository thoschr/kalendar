#include "categorydialog.h"

#include <QDebug>
#include <functional>

CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)
{
    this->setFixedWidth(300);
    this->setFixedHeight(400);
    this->setWindowTitle("Category Manager");
    this->custom_dialog = NULL;
    this->pm = new PManager;
    this->selected_color = QColor(DEFAULT_COLOR);
    QVBoxLayout *layout = new QVBoxLayout;
    this->list_widget = new QListWidget;
    connect(this->list_widget, &QListWidget::itemClicked, this, &CategoryDialog::on_item_click);
    load_categories();
    QPushButton *button_delete = new QPushButton("Delete");
    QPushButton *button_add = new QPushButton("Add new category");
    QPushButton *button_cancel = new QPushButton("Cancel");
    QHBoxLayout *hl = new QHBoxLayout;
    this->button_color = new QPushButton;
    QPixmap pixmap(ICON_SIZE, ICON_SIZE);
    pixmap.fill(QColor(DEFAULT_COLOR));
    button_color->setIcon(QIcon(pixmap));
    this->edit_name = new QLineEdit;
    this->edit_name->setPlaceholderText("Type a name for the new category");
    connect(button_color, &QPushButton::clicked, this, &CategoryDialog::on_button_color_click);
    connect(button_cancel, &QPushButton::clicked, this, &CategoryDialog::on_button_cancel_click);
    connect(button_add, &QPushButton::clicked, this, &CategoryDialog::on_button_add_click);
    connect(button_delete, &QPushButton::clicked, this, &CategoryDialog::on_button_delete_click);
    layout->addWidget(this->list_widget);
    hl->addWidget(this->edit_name);
    hl->addWidget(button_color);
    layout->addLayout(hl);
    layout->addWidget(button_add);
    layout->addWidget(button_cancel);
    layout->addWidget(button_delete);
    this->setLayout(layout);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
    delete pm;
    for (Category *category : this->category_list) delete category;
}

void CategoryDialog::load_categories() {
    this->category_list = this->pm->get_categories();
    this->list_widget->clear();
    for (Category *category : this->category_list) {
        QPixmap pixmap(ICON_SIZE, ICON_SIZE);
        pixmap.fill(QColor(category->getColor().c_str()));
        this->list_widget->addItem(new QListWidgetItem(QIcon(pixmap), QString(category->getName().c_str())));
    }
}

void CategoryDialog::on_item_click(QListWidgetItem *item) {
    QVBoxLayout *vl = new QVBoxLayout;
    QHBoxLayout *hl = new QHBoxLayout;
    QHBoxLayout *hl2 = new QHBoxLayout;
    QPushButton *button_color = new QPushButton;
    button_color->setIcon(item->icon());
    QLineEdit *edit_name = new QLineEdit(item->text());
    hl->addWidget(edit_name);
    hl->addWidget(button_color);
    QPushButton *button_save = new QPushButton("Save");
    QPushButton *button_delete = new QPushButton("Delete");
    connect(button_delete, &QPushButton::clicked, this, &CategoryDialog::on_button_delete_click);
    connect(button_save, &QPushButton::clicked, this, &CategoryDialog::on_button_save_click);
    hl2->addWidget(button_save);
    hl2->addWidget(button_delete);
    vl->addLayout(hl);
    vl->addLayout(hl2);
    this->custom_dialog = new CustomDialog(vl);
    this->custom_dialog->setModal(true);
    this->custom_dialog->show();
}

void CategoryDialog::on_button_cancel_click() {
    this->close();
    delete this;
}

void CategoryDialog::on_button_save_click() {
    //TODO: Implement this
}

void CategoryDialog::on_button_delete_click() {
    QString selected_category = this->list_widget->selectedItems().at(0)->text();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", QString("Do you want to delete ") + selected_category + QString("?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        for (Category *category : this->category_list) {
            if (category->getName() == selected_category.toStdString()) {
                if (!this->pm->remove_category(category))
                    QMessageBox::critical(this, "Error", "Persistence error. You can't remove a category used by events. First remove these events and then try again.");
                else
                    load_categories();
                break;
            }
        }
        this->custom_dialog->close();
        delete this->custom_dialog;
        this->custom_dialog = NULL;
    }
}

void CategoryDialog::on_button_add_click() {
    if(this->edit_name->text().length() < 3) {
        QMessageBox::critical(this, "Error", "The name must have a length greater than 2", QMessageBox::Ok);
        return;
    }
    Category category(0, this->edit_name->text().toStdString(), this->selected_color.name().toStdString());
    if (this->pm->add_category(&category)) {
        load_categories();
    } else {
        QMessageBox::critical(this, "Error", "Persistence error. Try with a different name or color.");
    }
}

void CategoryDialog::on_button_color_click() {
    this->selected_color = QColorDialog::getColor(QColor(DEFAULT_COLOR));
    QPixmap pixmap(ICON_SIZE, ICON_SIZE);
    pixmap.fill(QColor(this->selected_color));
    button_color->setIcon(QIcon(pixmap));
}
