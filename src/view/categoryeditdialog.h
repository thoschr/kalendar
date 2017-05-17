#ifndef CATEGORYEDITDIALOG_H
#define CATEGORYEDITDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include "ui_categoryeditdialog.h"
#include "model/category.h"
#include "categorydialog.h"

//We need this forward declaration to avoid circular dependencies among headers
class CategoryDialog;

namespace Ui {
class CategoryEditDialog;
}

class CategoryEditDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CategoryEditDialog *ui;
    CategoryDialog *parentDialog;
    QColor selected_color;
    QLineEdit *edit_name;
    QPushButton *button_color;

public:
    explicit CategoryEditDialog(CategoryDialog *parentDialog, QWidget *parent = 0);
    ~CategoryEditDialog();

public slots:
    void on_button_delete_click();
    void on_button_save_click();
    void on_button_color_click();
};

#endif
