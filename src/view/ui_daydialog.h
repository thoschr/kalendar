/********************************************************************************
** Form generated from reading UI file 'eventdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAYDIALOG_H
#define UI_DAYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DayDialog
{
public:

    void setupUi(QDialog *DayDialog)
    {
        if (DayDialog->objectName().isEmpty())
            DayDialog->setObjectName(QStringLiteral("DayDialog"));
        DayDialog->resize(400, 300);

        retranslateUi(DayDialog);

        QMetaObject::connectSlotsByName(DayDialog);
    } // setupUi

    void retranslateUi(QDialog *DayDialog)
    {
        DayDialog->setWindowTitle(QApplication::translate("DayDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class DayDialog: public Ui_DayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAYDIALOG_H
