#include "qlabel_event.h"

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent)
{
    this->event = NULL;
}

void QLabelEvent::mouseReleaseEvent(QMouseEvent *) {
    emit clicked(this->event);
}
