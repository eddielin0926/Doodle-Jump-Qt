#include "player.h"
#include "platform.h"
#include "bullet.h"
#include "hazard.h"
#include <QPixmap>
#include <QDebug>

Player::Player(int x, int y, DoodleJump * dj)
{
    vel = -30;
    doodlejump = dj;
    status = 1;
    count = 0;
    shield = false;
    isHit = false;
    props = NULL;

    mouth = new QGraphicsPixmapItem(QPixmap(":/player/images/player_Mouth.png"));
    mouth->setZValue(2);

    star_imgs[0].load(":/player/images/star1.png");
    star_imgs[1].load(":/player/images/star2.png");
    star_imgs[2].load(":/player/images/star3.png");

    star = new QGraphicsPixmapItem(star_imgs[0]);
    star->setZValue(2);

    imgs[0][0].load(":/player/images/player_L.png");
    imgs[0][1].load(":/player/images/player_Left_Jump.png");
    imgs[1][0].load(":/player/images/player_Right.png");
    imgs[1][1].load(":/player/images/player_Right_Jump.png");
    imgs[2][0].load(":/player/images/player_Shoot.png");
    imgs[2][1].load(":/player/images/player_Shoot_Jump.png");
    setPixmap(imgs[0][0]); // need to change
    setPos(x, y);
    setZValue(2);

    timer = new QTimer();
    timer->start(32);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));

    // make the player focusable and set it to be the current focus
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    static const int offset = pixmap().width() / 2; // make up the margin of picture
    static const int DISPL = 15; // Displacement

    // move left or right
    switch (event->key()) {
    case Qt::Key_A:
    case Qt::Key_Left:
        setX(pos().x() - DISPL < -pixmap().width() + offset? 600 - offset : pos().x() - DISPL);
        if(status != 0){
            if(status == 2){
                scene()->removeItem(mouth);
                count = 0;
            }
            status = 0;
            setPixmap(imgs[status][0]);
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        setX(pos().x() + DISPL > 600 - offset? -pixmap().width() + offset : pos().x() + DISPL);
        if(status != 1){
            if(status == 2){
                scene()->removeItem(mouth);
                count = 0;
            }
            status = 1;
            setPixmap(imgs[status][0]);
        }
        break;
    case Qt::Key_Space:
        if(status != 2 || count > 2){
            shoot();
        }
        break;
    case Qt::Key_Up:
        setY(pos().y() - DISPL);
        break;
    case Qt::Key_Down:
        setY(pos().y() + DISPL);
        break;
    default:
        break;
    }

     return;
}

void Player::setVel(float v)
{
    if(!isHit){
        setPixmap(imgs[status][1]);
        vel = v;
    }
}

void Player::start()
{
    setFocus();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

void Player::stop()
{
    clearFocus();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

int Player::getVel()
{
    return vel;
}

void Player::shoot()
{
    mouth->setPos(x() + 35, y());
    if(status != 2){
        scene()->addItem(mouth);
    }
    setPixmap(imgs[2][0]);
    Bullet * bullet = new Bullet(y() - 1000);
    bullet->setPos(x() + 35, y() - 5);
    scene()->addItem(bullet);
    status = 2;
}

void Player::hitByMonster()
{
    isHit = true;
    star->setPos(pos().x() + 10, pos().y());
    scene()->addItem(star);
}

void Player::hitByHole()
{
    if(pixmap().width() < 20){
        setPos(-100, -1000);
        start();
        doodlejump->isEnd = true;
        disconnect(doodlejump->timer, SIGNAL(timeout()), this, SLOT(hitByHole()));
        return;
    }
    setPixmap(QPixmap(imgs[status][0]).scaled(pixmap().width()-10, pixmap().height()-10,Qt::KeepAspectRatio));
    return;
}

void Player::reset()
{
    if(star->scene()){
        star->scene()->removeItem(star);
    }
    isHit = false;
    setPixmap(imgs[1][0]);
}

int Player::getStatus()
{
    return status;
}

void Player::move()
{
    static int t = 0;
    setPixmap(imgs[status][0]);

    // let the player drops
    vel += 1.5;
    if(!doodlejump->isEnd){
        if(vel > 33){
           vel = 33;
        }
    }
    if (isHit){
        setY(pos().y() + vel);
        star->setPixmap(star_imgs[t/2]);
        star->setPos(pos().x() + 10, pos().y());
        ++t;
        if(t > 5){
            t = 0;
        }
        return;
    }
    setY(pos().y() + vel);

    if(status == 2){
        mouth->setPos(x() + 35, y());
        if(count++ > 3){
            scene()->removeItem(mouth);
            count = 0;
            status = 1;
        }
    }

    // check the collision
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for(int i = 0, n = colliding_items.size();i<n; ++i){
        Platform * platform = dynamic_cast<Platform *>(colliding_items[i]);
        if(platform){
            platform->collide(this);
        }
        Props * props = dynamic_cast<Props *>(colliding_items[i]);
        if(props){
            props->collide(this);
        }
        Hazard * hazard = dynamic_cast<Hazard *>(colliding_items[i]);
        if(hazard){
            hazard->collide(this);
        }
    }

    return;
}
