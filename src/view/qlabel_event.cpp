#include "qlabel_event.h"

#include <QDebug>

QLabelEvent::QLabelEvent(QWidget *parent) : QLabel(parent)
{
    this->event = NULL;
}

QLabelEvent::~QLabelEvent() {
    delete this->event;
}

void QLabelEvent::mousePressEvent(QMouseEvent *e) {
    emit clicked(this, e->button());
}

void QLabelEvent::setEvent(Event *event) {
    if (this->event != NULL) delete this->event;
    this->event = event;
}

Event* QLabelEvent::getEvent() {
   return this->event;
}


