#include "props.h"

Props::Props():player(NULL){}

Spring::Spring()
{
    imgs[0].load(":/props/images/spring.png");
    imgs[1].load(":/props/images/spring_stretch.png");
    setZValue(1);
    setPixmap(imgs[0]);
}

Spring::~Spring()
{
    if(scene()){
        scene()->removeItem(this);
    }
}

void Spring::collide(Player * player)
{
    if(player->getVel() > 0){
        if(player->pos().y() + player->pixmap().height() <= pos().y() + pixmap().height()){
            player->setVel(-45);
            setPixmap(imgs[1]);
            setY(y() - 30);
        }
    }
}


PropellerHelmet::PropellerHelmet(Platform * p, QTimer * time)
{
    used = false;
    platform = p;
    timer = time;
    t = 0;
    count = 0;
    endPos = 0;
    setPixmap(QPixmap(":/props/images/propellerHelmet.png"));
    setZValue(3);

    imgs[0].load(":/props/images/propellerHelmet1.png");
    imgs[1].load(":/props/images/propellerHelmet2.png");
    imgs[2].load(":/props/images/propellerHelmet3.png");
}

PropellerHelmet::~PropellerHelmet()
{

}

void PropellerHelmet::collide(Player * p)
{
    if(!used){
        used = true;
        player = p;
        platform->props = NULL;
        player->stop();
        player->setFocus();
        connect(timer, SIGNAL(timeout()), this, SLOT(fly()));
    }
}

void PropellerHelmet::fly()
{
    player->setY(player->pos().y() - 20);
    setPixmap(imgs[count++]);
    if(count > 2){
        count = 0;
    }
    if(player->getStatus() == 0){
        setPos(player->pos().x() + 35, player->pos().y());
    } else if (player->getStatus() == 1) {
        setPos(player->pos().x() + 2, player->pos().y());
    }
    endPos = pos().y() + 1000;
    ++t;
    if(t > 50){
        disconnect(timer, SIGNAL(timeout()), this, SLOT(fly()));
        connect(timer, SIGNAL(timeout()), this, SLOT(fall()));
        player->setVel(-17);
        player->start();
    }
}

void PropellerHelmet::fall()
{
    setPos(pos().x() + 2, pos().y() + 33);
    if(pos().y() >= endPos){
        disconnect(timer, SIGNAL(timeout()), this, SLOT(fall()));
        delete this;
    }
}
