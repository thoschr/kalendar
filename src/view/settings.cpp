#include "settings.h"

#include <QDebug>

const QString SettingsValues::past_months_expiration = "PastMonthsExpiration";

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    this->setWindowTitle("Settings");
    QVBoxLayout *main_layout = new QVBoxLayout;
    QHBoxLayout *row1 = new QHBoxLayout;
    QLabel *lblPastMonthExpiration = new QLabel("Delete all events older than (months): ");
    this->pastMonthsExpiration = new QSpinBox;
    pastMonthsExpiration->setRange(0, 120);
    pastMonthsExpiration->setSingleStep(1);
    pastMonthsExpiration->setValue(2);
    pastMonthsExpiration->setFixedWidth(100);
    row1->addWidget(lblPastMonthExpiration);
    row1->addWidget(pastMonthsExpiration);
    QHBoxLayout *lastRow = new QHBoxLayout;
    QPushButton *btnCancel = new QPushButton("Cancel");
    QPushButton *btnSave = new QPushButton("Save");
    connect(btnCancel, &QPushButton::clicked, this, &Settings::on_button_cancel_click);
    connect(btnSave, &QPushButton::clicked, this, &Settings::on_button_save_click);
    lastRow->addWidget(btnCancel);
    lastRow->addWidget(btnSave);
    main_layout->addLayout(row1);
    main_layout->addLayout(lastRow);
    setLayout(main_layout);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_button_cancel_click() {
   this->close();
}

void Settings::on_button_save_click() {
    QSettings settings;
    settings.setValue(SettingsValues::past_months_expiration, this->pastMonthsExpiration->value());
    this->close();
}


