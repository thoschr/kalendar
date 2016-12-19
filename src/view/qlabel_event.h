#ifndef QLABELEVENT_H
#define QLABELEVENT_H

#include <QLabel>
#include <QMouseEvent>
#include "model/event.h"

class QLabelEvent : public QLabel
{
    Q_OBJECT
private:
    Event *event;

public:
    explicit QLabelEvent(QWidget *parent = 0);
    ~QLabelEvent();
    void setEvent(Event *event);
    Event* getEvent();
protected:
    virtual void mousePressEvent(QMouseEvent *e);

signals:
    void clicked(QLabelEvent *event, Qt::MouseButton button);

public slots:

};

#endif // QLABELEVENT_H
