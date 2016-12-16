#ifndef CATEGORYSELECTDIALOG_H
#define CATEGORYSELECTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "ui_categoryselectdialog.h"
#include "view.h"
#include "persistence/pmanager.h"
#include "categorydialog.h"

namespace Ui {
class CategorySelectDialog;
}

class CategorySelectDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CategorySelectDialog *ui;
    QComboBox *list_categories;
    vector<Category*> category_list;
    PManager *pm;
    View *parent;
    unsigned int selected_category;

public:
    explicit CategorySelectDialog(View *parentView, QWidget *parent = 0);
    ~CategorySelectDialog();
    void load_categories();
    unsigned int getSelectedCategory();

public slots:
    void on_button_cancel_click();
    void on_button_ok_click();
};

#endif // CATEGORYSELECTDIALOG_H
