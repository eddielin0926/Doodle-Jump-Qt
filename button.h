#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QPixmap>

class Button: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT;
public:
    Button(const char * = "", const char * = "", QGraphicsItem * parent = NULL);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

    QPixmap imgs[2];

signals:
    void clicked();
};

#endif // BUTTON_H
