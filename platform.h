#ifndef PLATFORM_H
#define PLATFORM_H

#include "player.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>

class Player;

class Platform: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Platform();
    virtual ~Platform(){};

    virtual void collide(Player*){};

public slots:
    virtual void move(){};

protected:
    QTimer * timer;
    float VEL;
};

class NormalPlatform: public Platform
{
    Q_OBJECT

public:
    NormalPlatform();
    virtual ~NormalPlatform(){};

    virtual void collide(Player*);
};

class CrackedPlatform: public Platform
{
    Q_OBJECT

public:
    CrackedPlatform();
    virtual ~CrackedPlatform();

    virtual void collide(Player*);

public slots:
    void falling();

protected:
    QPixmap img[4];
    int count;
};

class HorizontalMovePlatform: public NormalPlatform
{
    Q_OBJECT

public:
    HorizontalMovePlatform();
    virtual ~HorizontalMovePlatform();

public slots: // or private?
    virtual void move();
};

#endif // PLATFORM_H
