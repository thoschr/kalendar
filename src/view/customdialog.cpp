#include "customdialog.h"
#include "ui_customdialog.h"

#include <QDebug>

CustomDialog::CustomDialog(QLayout *layout, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setLayout(layout);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::keyPressEvent(QKeyEvent *e) {
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else {
        this->close();
        delete this;
    }
}
