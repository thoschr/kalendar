#ifndef VIEW_H
#define VIEW_H

#include "model/date.h"
#include "model/category.h"

/* Abstract Class */
class View
{
public:
    //pure virtual functions
    virtual void display_events(Date date) = 0;
    virtual void display_events(Date date, Category category) = 0;
};

#endif // VIEW_H
