#ifndef QWIDGETEXTENDED_H
#define QWIDGETEXTENDED_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include "qframe_extended.h"

class QWidgetExtended : public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetExtended(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

signals:
    void pressed(QFrameExtended *);
    void released(QFrameExtended *);

};

#endif
