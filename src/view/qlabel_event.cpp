#include "qlabel_event.h"

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent)
{
}

void QLabelEvent::mouseReleaseEvent(QMouseEvent *) {
    emit clicked(this->event);
}
