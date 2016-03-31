#ifndef DAYDIALOG_H
#define DAYDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include "view.h"
#include "qframe_extended.h"

namespace Ui {
class DayDialog;
}

class DayDialog : public QDialog
{
    Q_OBJECT
private:
    View *parentView;
    QFrameExtended *frame;

public:
    explicit DayDialog(View *parentView, QFrameExtended *frame, QWidget *parent = 0);
    ~DayDialog();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    Ui::DayDialog *ui;
};

#endif // DAYDIALOG_H
