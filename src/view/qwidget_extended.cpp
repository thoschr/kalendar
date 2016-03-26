#include "qwidget_extended.h"

#include <QMouseEvent>
#include "qframe_extended.h"
#include <QDebug>

QWidgetExtended::QWidgetExtended(QWidget *parent) : QWidget(parent)
{

}

void QWidgetExtended::mousePressEvent(QMouseEvent *event) {
    QFrameExtended frame;
    QWidget *widget = this->childAt(event->pos());
    if (widget != NULL) {
        QString widgetClassName(widget->metaObject()->className());
        //I don't use explicitly the string because if one day someone changes the name of the class, the compiler will output an error
        QString className(frame.metaObject()->className());
        if (widgetClassName == className) {
            emit pressed(dynamic_cast<QFrameExtended*> (widget));
        }
    }
}

void QWidgetExtended::mouseReleaseEvent(QMouseEvent *event) {
    QFrameExtended frame;
    QWidget *widget = this->childAt(event->pos());
    if (widget != NULL) {
        QString widgetClassName(widget->metaObject()->className());
        //I don't use explicitly the string because if one day someone changes the name of the class, the compiler will output an error
        QString className(frame.metaObject()->className());
        if (widgetClassName == className) {
            emit released(dynamic_cast<QFrameExtended*> (widget));
        }
    }
}
