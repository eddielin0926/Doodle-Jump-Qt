#include "hazard.h"

Monster::Monster(QTimer * t)
{
    timer = t;
    setPixmap(QPixmap(":/hazard/images/monster1.png"));
}

Monster::~Monster()
{
    if(scene()){
        scene()->removeItem(this);
    }
}

void Monster::collide(Player * player)
{
    if (player->getVel() > 0) {
        if(player->pos().y() < this->pos().y()){
            connect(timer, SIGNAL(timeout()), this, SLOT(move()));
        }
        player->setVel();
    } else {
        player->setVel(0);
        player->hitByMonster();
    }
}

void Monster::hit()
{
    scene()->removeItem(this);
}

void Hazard::move()
{
    setY(pos().y() + 33);
}

Hole::Hole(QTimer * t)
{
    timer = t;
    setPixmap(QPixmap(":/hazard/images/hole.png"));
}

Hole::~Hole()
{
    if(scene()){
        scene()->removeItem(this);
    }
}

void Hole::collide(Player * player)
{
    player->stop();
    player->setPos(pos().x() + 50, pos().y() + 50);
    player->hitByHole();
    connect(timer, SIGNAL(timeout()), player, SLOT(hitByHole()));
}
