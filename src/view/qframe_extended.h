#ifndef QFRAMEEXTENDED_H
#define QFRAMEEXTENDED_H

#include <QObject>
#include <QFrame>
#include "util/timeutil.h"

class QFrameExtended : public QFrame
{
    Q_OBJECT

private:
    Time time;
public:
    explicit QFrameExtended(Time time, QWidget *parent = 0);
    Time getTime() { return time; }

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
