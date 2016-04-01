#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include "ui_categorydialog.h"
#include "persistence/pmanager.h"
#include "customdialog.h"

#define DEFAULT_COLOR "#1022A0"
#define ICON_SIZE 10

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CategoryDialog *ui;
    QListWidget *list_widget;
    list<Category*> category_list;
    PManager *pm;
    QColor selected_color;
    QLineEdit *edit_name;
    QPushButton *button_color;
    CustomDialog *custom_dialog;
    void load_categories();

public:
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();

public slots:
    void on_button_cancel_click();
    void on_button_add_click();
    void on_button_delete_click();
    void on_button_save_click();
    void on_button_color_click();
    void on_item_click(QListWidgetItem *item);
};

#endif // CATEGORYDIALOG_H
