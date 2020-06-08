#include "button.h"

Button::Button(const char * img1, const char * img2, QGraphicsItem * parent)
{
    imgs[0].load(img1);
    imgs[1].load(img2);

    setPixmap(imgs[0]);

    // allow to detect mouse hover
    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    emit clicked();
    setPixmap(imgs[0]);
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    setPixmap(imgs[1]);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    setPixmap(imgs[0]);
}
