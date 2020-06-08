#ifndef PLAYER_H
#define PLAYER_H

#include "doodlejump.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

class DoodleJump;

class Player:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Player(int, int, DoodleJump * = NULL);

    void keyPressEvent(QKeyEvent * event);
    void setVel(float = -30);
    void start();
    void stop();
    int getVel();

public slots:
    void move();

private:
    float vel;
    int status;
    QPixmap imgs[3][2];
    QTimer * timer;
    DoodleJump * doodlejump;
};

#endif // PLAYER_H
