#ifndef QFRAMEEXTENDED_H
#define QFRAMEEXTENDED_H

#include <QObject>
#include <QFrame>
#include "util/dateutil.h"

class QFrameExtended : public QFrame
{
    Q_OBJECT

private:
    Date *date;

public:
    explicit QFrameExtended(QWidget *parent = 0) : QFrame(parent) {}
    Date* getDate() { return date; }
    void setDate(Date *d) { this->date = d; }

};

#endif // QFRAMEEXTENDED_H
