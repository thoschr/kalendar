#ifndef QFRAMEEXTENDED_H
#define QFRAMEEXTENDED_H

#include <QObject>
#include <QFrame>
#include "util/dateutil.h"

class QFrameExtended : public QFrame
{
    Q_OBJECT

private:
    Date date;

public:
    explicit QFrameExtended(QWidget *parent = 0);
    Date getDate() { return date; }
    void setDate(Date d) { this->date = d; }

signals:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void pressed(QFrameExtended *);
    void released(QFrameExtended *);

public slots:
    void on_mouse_press();
    void on_mouse_release();
};

#endif // QFRAMEEXTENDED_H
