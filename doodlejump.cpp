#include "doodlejump.h"
#include <ctime>
#include <random>
#include <QPoint>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>

DoodleJump::DoodleJump(QGraphicsView * v)
{    
    player = new Player(80, 900, this); // need change
    player->setVel(-43);

    score = 0;
    highestScore = 0;
    isEnd = false;
    name = "Doodle Jump";

    scene = new QGraphicsScene();

    view = v;
    view->setScene(scene);
    view->setSceneRect(0, 0, 640, 960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer();
    timer->start(100);

    welcomeBackground.setPixmap(QPixmap(":/background/images/welcome.png"));
    welcomeBackground.setPos(0, 0);
    pauseBackground.setPixmap(QPixmap(":/background/images/pause.png"));
    pauseBackground.setZValue(2);
    background[0].setPixmap(QPixmap(":/background/images/background.png"));
    background[0].setPos(0, 0);
    background[1].setPixmap(QPixmap(":/background/images/background.png"));
    background[1].setPos(0, -background[1].pixmap().height());
    toolBar.setPixmap(QPixmap(":/background/images/toolbar.png"));
    toolBar.setPos(0, 0);
    toolBar.setZValue(2);
    endBar.setPixmap(QPixmap(":/background/images/endBar.png"));
    endBar.setPos(0, 0);
    endBar.setZValue(2);
    gameOver.setPixmap(QPixmap(":/background/images/end.png"));
    gameOver.setPos(110, 0);
    gameOver.setZValue(2);

    playButton = new Button(":/button/images/play_button.png", ":/button/images/play_button_clicked.png");
    playButton->setPos(130, 230);
    connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
    pauseButton = new Button(":/button/images/pauseButton.png", ":/button/images/pauseButton.png");
    pauseButton->setPos(570, 30);
    pauseButton->setZValue(2);
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    resumeButton = new Button(":/button/images/resumeButton.png", ":/button/images/resumeButton_clicked.png");
    resumeButton->setPos(350, 700);
    resumeButton->setZValue(2);
    connect(resumeButton, SIGNAL(clicked()), this, SLOT(resume()));
    playAgainButton = new Button(":/button/images/playAgainButton.png", ":/button/images/playAgainButton_clicked.png");
    playAgainButton->setPos(220, 0);
    playAgainButton->setZValue(2);
    connect(playAgainButton, SIGNAL(clicked()), this, SLOT(play()));
    menuButton = new Button(":/button/images/menuButton.png", ":/button/images/menuButton_clicked.png");
    menuButton->setPos(400, 0);

    NormalPlatform * platform = new NormalPlatform;
    platform->setPos(57, 745);
    platforms.push_back(platform);

    int id = QFontDatabase::addApplicationFont(":/font/resource/DoodleJump.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont alSeana(family);
    alSeana.setBold(true);
    alSeana.setPixelSize(64);

    scoreText = new QGraphicsTextItem;
    scoreText->setFont(alSeana);
    scoreText->setPlainText("0");
    scoreText->setPos(20, 10);
    scoreText->setZValue(2);

    alSeana.setPixelSize(48);
    endText = new QGraphicsTextItem;
    endText->setFont(alSeana);
    endText->setPlainText("     your score: 0\nyour high score: 0\n   your name: Doodle Jump");
    endText->setPos(90, 0);
    endText->setZValue(2);

    // add item to scence
    scene->addItem(&welcomeBackground);
    scene->addItem(playButton);
    scene->addItem(platform);
    scene->addItem(player);

    std::srand(time(NULL));
}

void DoodleJump::play()
{
    score = 0;
    last = 0;

    view->setSceneRect(0, 0, 640, 960);
    last = view->mapToScene(QPoint(0, view->height() / 2)).y();

    player->setFocus();
    player->setPos(250, 700);

    if(!isEnd){
        scene->removeItem(&welcomeBackground);
        scene->addItem(&background[0]);
        scene->addItem(&background[1]);
        scene->addItem(&toolBar);
        scene->addItem(pauseButton);
        scene->addItem(scoreText);
    } else {
        connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
        scene->removeItem(&gameOver);
        scene->removeItem(endText);
        scene->removeItem(playAgainButton);
        scene->removeItem(menuButton);
        scene->removeItem(&endBar);

        NormalPlatform * platform = new NormalPlatform;
        platforms.push_back(platform);
        scene->addItem(platform);
    }

    platforms[0]->setPos(250, 830);

    spawnPlaform(background[0].pos().y());
    spawnPlaform(background[1].pos().y());

    isEnd = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    player->setFocus();
}

void DoodleJump::pause()
{
    player->stop();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    resumeButton->setY(view->mapToScene(QPoint(0, 700)).y());
    pauseBackground.setY(view->mapToScene(QPoint(0, toolBar.pixmap().height())).y() - 5);

    scene->addItem(&pauseBackground);
    scene->addItem(resumeButton);
}

void DoodleJump::resume()
{
    player->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    scene->removeItem(resumeButton);
    scene->removeItem(&pauseBackground);
    player->setFocus();
}

void DoodleJump::setBackGround()
{
    static int endPos = 0;

    cleanPlatform();

    if(isEnd){
        if(view->mapToScene(QPoint(0, 0)).y() >= endPos){
            end();
        }
        view->setSceneRect(0, view->mapToScene(QPoint(0, 100)).y(), 640, 960);
        toolBar.setY(view->mapToScene(QPoint(0, 0)).y());
        pauseButton->setY(view->mapToScene(QPoint(0, 30)).y());
        scoreText->setY(view->mapToScene(QPoint(0, 10)).y());
        if(background[0].pos().y() + background[0].pixmap().height() < view->mapToScene(QPoint(0, 0)).y()){
            background[0].setY(background[1].pos().y() + background[1].pixmap().height());
        }
        if(background[1].pos().y() + background[1].pixmap().height() < view->mapToScene(QPoint(0, 0)).y()){
            background[1].setY(background[0].pos().y() + background[0].pixmap().height());
        }
        return;
    } else {
        endPos = view->mapToScene(QPoint(0, 0)).y() + background[0].pixmap().height();
    }

    // keep the player's highest spot in the middle of the view
    if(player->pos().y() < last){
        view->setSceneRect(0, player->pos().y() - view->height() / 2, 640, 1024);
        last = player->pos().y();
    }

    toolBar.setY(view->mapToScene(QPoint(0, 0)).y());
    pauseButton->setY(view->mapToScene(QPoint(0, 30)).y());
    scoreText->setY(view->mapToScene(QPoint(0, 10)).y());

    // move the background which is out of view
    if(background[0].pos().y() > view->mapToScene(QPoint(0, view->height())).y()){
        background[0].setY(background[1].pos().y() - background[0].pixmap().height());
        spawnPlaform(background[0].pos().y());
    }
    if(background[1].pos().y() > view->mapToScene(QPoint(0, view->height())).y()){
        background[1].setY(background[0].pos().y() - background[1].pixmap().height());
        spawnPlaform(background[1].pos().y());
    }

    if(player->pos().y() > view->mapToScene(QPoint(0, view->height())).y()){
        isEnd = true;
    }

    score = -(view->mapToScene(QPoint(0, 0)).y());
    if(score < 0){
        score = 0;
    }
    scoreText->setPlainText(QString::number(score));
}


void DoodleJump::spawnPlaform(int begin)
{
//    int least = 10;
    static int most = 15;

    Platform * platform;
    for(int i = 0; i < most; ++i){
        int p = rand() % 100;
        if(p >= 20) {
            platform = new NormalPlatform;
        } else if (p >= 15) {
            platform = new CrackedPlatform;
        } else if (p >= 0) {
            platform = new HorizontalMovePlatform;
        }

        int count = 0;
        do{
            platform->setPos(rand() % 470, rand() % 920 + begin);
            if(count++ > 1000){
                break;
            }
        } while (isOverlapping(platform));
        if(count < 1000) {
            scene->addItem(platform);
            platforms.push_back(platform);
        } else {
            delete platform;
            break;
        }
    }
}

bool DoodleJump::isOverlapping(Platform * target)
{
    for(auto plat: platforms){
        if(target->pos().y() <= plat->pos().y() + plat->pixmap().height() && \
           target->pos().y() >= plat->pos().y()){
            return true;
        }
        if(target->pos().y() + target->pixmap().height() <= plat->pos().y() + plat->pixmap().height() && \
           target->pos().y() + target->pixmap().height() >= plat->pos().y()){
            return true;
        }
    }
    return false;
}

void DoodleJump::cleanPlatform()
{
    for(QVector<Platform *>::iterator iter = platforms.begin(); iter != platforms.end();){
        // kind of weird
        auto platform = *iter;
        if(platform->pos().y() >  view->mapToScene(QPoint(0, view->height())).y()){
            scene->removeItem(platform);
            platforms.erase(iter);
            delete platform;
        } else {
            iter++;
        }
    }
}

void DoodleJump::end()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    disconnect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    endBar.setY(view->mapToScene(QPoint(0, view->height())).y() - 33);
    gameOver.setY(view->mapToScene(QPoint(0, 0)).y() + 330);
    if(score > highestScore){
        highestScore = score;
    }
    QString text;
    text += "     your score: " + QString::number(score);
    text += "\nyour high score: " + QString::number(highestScore);
    text += "\n   your name: " + name;
    endText->setPlainText(text);
    endText->setY(view->mapToScene(QPoint(0, 0)).y() + 480);
    playAgainButton->setY(view->mapToScene(QPoint(0, 0)).y() + 720);
    menuButton->setY(view->mapToScene(QPoint(0, 0)).y() + 800);

    scene->addItem(&endBar);
    scene->addItem(&gameOver);
    scene->addItem(endText);
    scene->addItem(playAgainButton);
    scene->addItem(menuButton);

    for(auto platform: platforms){
        delete platform;
    }
    platforms.clear();
}
