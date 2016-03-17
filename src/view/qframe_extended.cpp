#include "qframe_extended.h"

#include <QDebug>

QFrameExtended::QFrameExtended(Time time, QWidget *parent) : QFrame(parent), time(time)
{
    connect( this, SIGNAL( mousePressEvent(QMouseEvent*) ), this, SLOT( on_mouse_press() ) );
    connect( this, SIGNAL( mouseReleaseEvent(QMouseEvent*) ), this, SLOT( on_mouse_release() ) );
}

void QFrameExtended::on_mouse_press() {
    emit pressed(this);
}

void QFrameExtended::on_mouse_release() {
    emit released(this);
}
