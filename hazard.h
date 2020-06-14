#ifndef HAZARD_H
#define HAZARD_H

#include "player.h"
#include "props.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDebug>

class Hazard: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Hazard(){};
    virtual ~Hazard(){};

    virtual void collide(Player *){};
    virtual void hit(){};

public slots:
    virtual void move();

protected:
    QTimer * timer;
};

class Monster: public Hazard
{
    Q_OBJECT

public:
    Monster(QTimer *);
    virtual ~Monster();

    virtual void collide(Player *) override;
    virtual void hit() override;
};

class Hole: public Hazard
{
    Q_OBJECT

public:
    Hole(QTimer *);
    virtual ~Hole();

    virtual void collide(Player *) override;
};

#endif // HAZARD_H
