#ifndef PLAYER_H
#define PLAYER_H

#include "doodlejump.h"
#include "props.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>
#include <QDebug>

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
    void shoot();
    void hitByMonster();
    void reset();
    int getStatus();
    int getVel();

    bool shield, isHit, inHole;
    Props * props;

public slots:
    void move();
    void hitByHole();

private:
    float vel;
    int status, count;
    QPixmap imgs[3][2], star_imgs[3];
    QTimer * timer;
    QGraphicsPixmapItem * mouth, * star;
    DoodleJump * doodlejump; // not necessary
};

#endif // PLAYER_H
