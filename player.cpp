#include "player.h"
#include "platform.h"
#include <QPixmap>
#include <QDebug>

Player::Player(int x, int y, DoodleJump * dj)
{
    vel = -30;
    doodlejump = dj;
    status = 1;

    imgs[0][0].load(":/player/images/player_L.png");
    imgs[0][1].load(":/player/images/player_Left_Jump.png");
    imgs[1][0].load(":/player/images/player_Right.png");
    imgs[1][1].load(":/player/images/player_Right_Jump.png");
    setPixmap(imgs[0][0]); // need to change
    setPos(x, y);
    setZValue(2);

    timer = new QTimer();
    timer->start(100);
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
            status = 0;
            setPixmap(imgs[status][0]);
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        setX(pos().x() + DISPL > 600 - offset? -pixmap().width() + offset : pos().x() + DISPL);
        if(status != 1){
           status = 1;
            setPixmap(imgs[status][0]);
        }
        break;
    case Qt::Key_Space:
        vel = -35;
        break;
    case Qt::Key_Up:
        setY(pos().y() - DISPL);
        break;
    case Qt::Key_Down:
        setY(pos().y() + DISPL);
    default:
        break;
    }

     return;
}

void Player::setVel(float v)
{
    setPixmap(imgs[status][1]);
    vel = v;
}

void Player::start()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

void Player::stop()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

int Player::getVel()
{
    return vel;
}

void Player::move()
{
    setPixmap(imgs[status][0]);
    // check the collision
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for(int i = 0, n = colliding_items.size();i<n; ++i){
        Platform * platform = dynamic_cast<Platform *>(colliding_items[i]);
        if(platform){
            platform->collide(this);
        } else {
            continue;
        }
    }

    // let the player drops
    vel += 1.5;
    if(!doodlejump->isEnd){
        if(vel > 33){
           vel = 33;
        }
    }
    setY(pos().y() + vel);

    return;
}
