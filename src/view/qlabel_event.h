#ifndef QLABELEVENT_H
#define QLABELEVENT_H

#include <QLabel>
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
    virtual void mouseReleaseEvent(QMouseEvent *);

signals:
    void clicked(Event *event);

public slots:

};

#endif // QLABELEVENT_H
