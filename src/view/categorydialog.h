#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include "ui_categorydialog.h"
#include "persistence/pmanager.h"
#include "categoryeditdialog.h"
#include "view.h"

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
    vector<Category*> category_list;
    PManager *pm;
    View *parent;

public:
    explicit CategoryDialog(View *parentView, QWidget *parent = 0);
    ~CategoryDialog();
    QListWidget *getListWidget();
    vector<Category*> getCategoryList();
    PManager* getPManager();
    View* getParentView();
    void load_categories();

public slots:
    void on_button_cancel_click();
    void on_button_add_click();
    void on_item_click();
};

#endif // CATEGORYDIALOG_H
