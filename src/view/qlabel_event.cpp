#include "qlabel_event.h"

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent)
{
    this->event = NULL;
}

QLabelEvent::~QLabelEvent() {
    delete this->event;
}

void QLabelEvent::mouseReleaseEvent(QMouseEvent *) {
    emit clicked(this->event);
}

void QLabelEvent::setEvent(Event *event) {
    if (this->event != NULL) delete this->event;
    this->event = event;
}

Event* QLabelEvent::getEvent() {
   return this->event;
}


