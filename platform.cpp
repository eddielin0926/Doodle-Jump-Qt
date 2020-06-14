#include "platform.h"
#include <QPixmap>
#include <QDebug>

Platform::Platform(): props(NULL), timer(NULL), VEL(0){}

NormalPlatform::NormalPlatform(QTimer * time)
{
    timer = time;
    setPixmap(QPixmap(":/platform/images/Normal_Platform.png", "PNG"));
    setZValue(1);
}

NormalPlatform::~NormalPlatform()
{
    if(scene()){
        scene()->removeItem(this);
    }
    if(props){
        delete props;
    }
}

void NormalPlatform::collide(Player * player)
{
    if(player->getVel() > 0){
        if(player->pos().y() + player->pixmap().height() >= pos().y() && \
           player->pos().y() + player->pixmap().height() <= pos().y() + pixmap().height())
        player->setVel();
    }
}

void NormalPlatform::spawnProps()
{
    int p = rand() % 100;
    if(p >= 30){
        props = new Spring();
    } else if (p >= 0) {
        props = new PropellerHelmet(this, timer);
    }

}

CrackedPlatform::CrackedPlatform()
{
    img[0] = QPixmap(":/platform/images/Cracked_Platform1.png", "PNG");
    img[1] = QPixmap(":/platform/images/Cracked_Platform2.png", "PNG");
    img[2] = QPixmap(":/platform/images/Cracked_Platform3.png", "PNG");
    img[3] = QPixmap(":/platform/images/Cracked_Platform4.png", "PNG");

    setPixmap(img[0]);
    setZValue(1);

    timer = new QTimer;

    VEL = 20;

    count = 0;
}

CrackedPlatform::~CrackedPlatform()
{
    if(timer!=NULL){
        disconnect(timer, SIGNAL(timeout()), this, SLOT(falling()));
    }
}

void CrackedPlatform::collide(Player * player)
{
    if(player->getVel() > 0){
        if(player->pos().y() + player->pixmap().height() >= pos().y() && \
           player->pos().y() + player->pixmap().height() <= pos().y() + pixmap().height())
        timer->start(100);
        connect(timer, SIGNAL(timeout()), this, SLOT(falling()));
    }

}

void CrackedPlatform::falling()
{
    if(count < 4){
        setPixmap(img[count]);
        ++count;
    }
    setY(pos().y() + VEL);
}

HorizontalMovePlatform::HorizontalMovePlatform(QTimer * time):NormalPlatform(time)
{
    setPixmap(QPixmap(":/platform/images/Horiziontal_Move_Platform.png", "PNG"));
    setZValue(1);

    connect(timer, SIGNAL(timeout()), this, SLOT(move()));

    VEL = 10;
}

HorizontalMovePlatform::~HorizontalMovePlatform()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

void HorizontalMovePlatform::move()
{
    if(pos().x() + pixmap().width() + VEL > 640 || pos().x() + VEL < 0){
        VEL *= -1;
    }
    setX(pos().x() + VEL);
}
