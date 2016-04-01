#include "daydialog.h"
#include "ui_daydialog.h"

#include <QDebug>

DayDialog::DayDialog(View *parentView, QFrameExtended *frame, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayDialog)
{
    this->parentView = parentView;
    this->frame = frame;

    this->setFixedWidth(300);
    this->setMinimumHeight(400);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    this->setWindowTitle("Day Dialog");

    QVBoxLayout *main_layout = new QVBoxLayout;

    main_layout->addWidget(frame);
    setLayout(main_layout);
}

DayDialog::~DayDialog()
{
    delete ui;
}

void DayDialog::keyPressEvent(QKeyEvent *e) {
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else {
        this->close();
        delete this;
    }
}
