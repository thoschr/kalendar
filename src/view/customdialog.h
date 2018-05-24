#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include "ui_customdialog.h"
#include <QDialog>
#include <QKeyEvent>
#include <QLabel>

namespace Ui {
class CustomDialog;
}

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QLayout *layout, QWidget *parent = 0);
    ~CustomDialog();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    Ui::CustomDialog *ui;
};

#endif
