#include "doodlejump.h"
#include <ctime>
#include <random>
#include <QPoint>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>

DoodleJump::DoodleJump(QGraphicsView * v)
{
    timer = new QTimer();
    timer->start(20);

    player = new Player(80, 900, timer, this);
    player->setVel(-43);

    score = 0;
    highestScore = 0;
    isEnd = false;
    name = "Doodler";
    turnOnSound = true;

    scene = new QGraphicsScene();

    view = v;
    view->setScene(scene);
    view->setSceneRect(0, 0, 640, 960);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    welcomeBackground = new QGraphicsPixmapItem();
    welcomeBackground->setPixmap(QPixmap(":/background/images/welcome.png"));
    welcomeBackground->setPos(0, 0);
    welcomeBackground->setZValue(0);
    pauseBackground = new QGraphicsPixmapItem();
    pauseBackground->setPixmap(QPixmap(":/background/images/pause.png"));
    pauseBackground->setZValue(6);
    scoreBackground = new QGraphicsPixmapItem();
    scoreBackground->setPixmap(QPixmap(":/background/images/scoreboard.png"));
    scoreBackground->setPos(0, 0);
    scoreBackground->setZValue(2);
    background[0].setPixmap(QPixmap(":/background/images/background.png"));
    background[0].setPos(0, 0);
    background[0].setZValue(0);
    background[1].setPixmap(QPixmap(":/background/images/background.png"));
    background[1].setPos(0, -background[1].pixmap().height());
    background[1].setZValue(0);
    optionBar = new QGraphicsPixmapItem();
    optionBar->setPixmap(QPixmap(":/background/images/option_bar.png"));
    optionBar->setPos(60, 50);
    optionBar->setZValue(1);
    toolBar = new QGraphicsPixmapItem();
    toolBar->setPixmap(QPixmap(":/background/images/toolbar.png"));
    toolBar->setPos(0, 0);
    toolBar->setZValue(8);
    endBar = new QGraphicsPixmapItem();
    endBar->setPixmap(QPixmap(":/background/images/endBar.png"));
    endBar->setPos(0, 0);
    endBar->setZValue(3);
    gameOver = new QGraphicsPixmapItem();
    gameOver->setPixmap(QPixmap(":/background/images/end.png"));
    gameOver->setPos(110, 0);
    gameOver->setZValue(2);
    tapToChange = new QGraphicsPixmapItem();
    tapToChange->setPixmap(QPixmap(":/background/images/tapToChange.png"));
    tapToChange->setPos(390, 0);
    tapToChange->setZValue(3);
    soundText = new QGraphicsPixmapItem();
    soundText->setPixmap(QPixmap(":/background/images/sounds.png"));
    soundText->setPos(300, 420);
    soundText->setZValue(3);

    playButton = new Button(":/button/images/play_button.png", ":/button/images/play_button_clicked.png");
    playButton->setPos(130, 230);
    connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
    pauseButton = new Button(":/button/images/pauseButton.png", ":/button/images/pauseButton.png");
    pauseButton->setPos(570, 30);
    pauseButton->setZValue(9);
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    resumeButton = new Button(":/button/images/resumeButton.png", ":/button/images/resumeButton_clicked.png");
    resumeButton->setPos(350, 700);
    resumeButton->setZValue(7);
    connect(resumeButton, SIGNAL(clicked()), this, SLOT(resume()));
    playAgainButton = new Button(":/button/images/playAgainButton.png", ":/button/images/playAgainButton_clicked.png");
    playAgainButton->setPos(220, 0);
    playAgainButton->setZValue(3);
    connect(playAgainButton, SIGNAL(clicked()), this, SLOT(play()));
    menuButton = new Button(":/button/images/menuButton.png", ":/button/images/menuButton_clicked.png");
    menuButton->setPos(400, 0);
    menuButton->setZValue(3);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(menu()));
    optionButton = new Button(":/button/images/option.png", ":/button/images/option_clicked.png");
    optionButton->setPos(500, 730);
    optionButton->setZValue(3);
    connect(optionButton, SIGNAL(clicked()), this, SLOT(option()));
    scoreButton = new Button(":/button/images/score_button.png", ":/button/images/score_button_clicked.png");
    scoreButton->setPos(400, 730);
    scoreButton->setZValue(3);
    connect(scoreButton, SIGNAL(clicked()), this, SLOT(scores()));

    OnOffButton = new Option(":/button/images/on.png", ":/button/images/off.png");
    OnOffButton->setPos(300, 470);
    OnOffButton->setZValue(3);
    connect(OnOffButton, SIGNAL(clicked()), this, SLOT(switchSound()));
    localButton = new Option(":/button/images/local_selected.png", ":/button/images/local.png");
    localButton->setPos(490, 752);
    localButton->setZValue(3);

    NormalPlatform * platform = new NormalPlatform(timer);
    platform->setPos(57, 745);
    platforms.push_back(platform);

    int id = QFontDatabase::addApplicationFont(":/font/resource/DoodleJump.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    font.setPixelSize(64);

    scoreText = new QGraphicsTextItem;
    scoreText->setFont(font);
    scoreText->setPlainText("0");
    scoreText->setPos(20, 10);
    scoreText->setZValue(9);

    font.setPixelSize(48);
    endText = new QGraphicsTextItem;
    endText->setFont(font);
    endText->setPlainText("     your score: 0\nyour high score: 0\n   your name: ");
    endText->setPos(90, 0);
    endText->setZValue(3);

    nameText = new QGraphicsTextItem;
    nameText->setFont(font);
    nameText->setPlainText(name);
    nameText->setPos(360, 0);
    nameText->setZValue(3);
    nameText->setTextInteractionFlags(Qt::TextEditorInteraction);

    // add item to scence
    scene->addItem(&background[0]);
    scene->addItem(&background[1]);
    scene->addItem(welcomeBackground);
    scene->addItem(playButton);
    scene->addItem(platform);
    scene->addItem(player);
    scene->addItem(optionButton);
    scene->addItem(scoreButton);

    endSound = new QMediaPlayer();
    endSound->setMedia(QUrl("qrc:/sound/resource/end.mp3"));

    scoreData = new Score();
//    scoreData->read();

    std::srand(time(NULL));
}

void DoodleJump::play()
{
    score = 0;
    last = 0;
    name = nameText->toPlainText();

    view->setSceneRect(0, 0, 640, 960);
    last = view->mapToScene(QPoint(0, view->height() / 2)).y();

    player->setFocus();
    player->setPos(250, 700);
    player->reset();

    cleanScene();
    scene->addItem(&background[0]);
    scene->addItem(&background[1]);
    scene->addItem(toolBar);
    scene->addItem(scoreText);
    scene->addItem(pauseButton);
    scene->addItem(player);

    if(isEnd) {
        connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));

        NormalPlatform * platform = new NormalPlatform(timer);
        platforms.push_back(platform);
        // scene->addItem(platform);
    }

    background[0].setPos(0, 0);
    background[1].setPos(0, -background[1].pixmap().height());

    platforms[0]->setPos(250, 830);
    scene->addItem(platforms[0]);

    spawnPlaform(background[0].pos().y());
    spawnPlaform(background[1].pos().y());

    scoreData->addToScene(scene);

    isEnd = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    player->setFocus();
}

void DoodleJump::pause()
{
    timer->stop();
    player->stop();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    resumeButton->setY(view->mapToScene(QPoint(0, 700)).y());
    pauseBackground->setY(view->mapToScene(QPoint(0, toolBar->pixmap().height())).y() - 5);

    if(!pauseBackground->scene()){
        scene->addItem(pauseBackground);
    }
    if(!resumeButton->scene()){
        scene->addItem(resumeButton);
    }
}

void DoodleJump::resume()
{
    timer->start(20);
    player->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    scene->removeItem(resumeButton);
    scene->removeItem(pauseBackground);
    player->setFocus();
}

void DoodleJump::menu()
{
    player->reset();
    player->setPos(80, 900);
    player->setVel(-43);

    score = 0;
    last = 0;
    isEnd = false;
    name = nameText->toPlainText();

    view->setSceneRect(0, 0, 640, 960);
    last = view->mapToScene(QPoint(0, view->height() / 2)).y();

    cleanScene();

    NormalPlatform * platform = new NormalPlatform(timer);
    platform->setPos(57, 745);
    platforms.push_back(platform);

    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));

    scene->addItem(welcomeBackground);
    scene->addItem(playButton);
    scene->addItem(platform);
    scene->addItem(player);
    scene->addItem(optionButton);
    scene->addItem(scoreButton);
}

void DoodleJump::option()
{
    cleanScene();

    menuButton->setPos(360, 750);

    background[0].setPos(0, 0);
    scene->addItem(&background[0]);
    scene->addItem(optionBar);
    scene->addItem(soundText);
    scene->addItem(OnOffButton);
    scene->addItem(menuButton);
}

void DoodleJump::scores()
{
    cleanScene();

    menuButton->setPos(400, 850);
    scene->addItem(scoreBackground);
    scene->addItem(menuButton);
    scene->addItem(localButton);

    scoreData->addRankToScene(scene);
}

void DoodleJump::switchSound()
{
    turnOnSound = !turnOnSound;
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
        toolBar->setY(view->mapToScene(QPoint(0, 0)).y());
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

    toolBar->setY(view->mapToScene(QPoint(0, 0)).y());
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
        if(turnOnSound) {
            if(endSound->state() == QMediaPlayer::PlayingState){
                endSound->setPosition(0);
            } else if(endSound->state() == QMediaPlayer::StoppedState) {
                endSound->play();
            }
        }
        isEnd = true;
    }

    score = -(view->mapToScene(QPoint(0, 0)).y());
    if(score < 0){
        score = 0;
    }
    scoreText->setPlainText(QString::number(score));
}

void DoodleJump::cleanScene()
{
    QList<QGraphicsItem *> list = scene->items();
    for(auto i: list){
        if(i->scene()){
            scene->removeItem(i);
        }
    }
    return;
}

bool DoodleJump::getTurnOnSound()
{
    return turnOnSound;
}

void DoodleJump::spawnPlaform(int begin)
{
//    int least = 10;
    static int most = 15;
    int count = 0;

    Platform * platform;
    for(int i = 0; i < most; ++i){
        int p = rand() % 100;
        if(p >= 30) {
            int q = rand() % 100;
            platform = new NormalPlatform(timer);
            if(q >= 80){
                platform->spawnProps();
            }
        } else if (p >= 20) {
            platform = new CrackedPlatform(timer);
        } else if (p >= 15) {
            platform = new OneOffPlatform(timer);
        } else if (p >= 0) {
            platform = new HorizontalMovePlatform(timer);
        }

        count = 0;
        do{
            platform->setPos(rand() % 470, rand() % 920 + begin);
            if(count++ > 1000){
                break;
            }
        } while (isOverlapping(platform));
        if(count < 1000) {
            scene->addItem(platform);
            platforms.push_back(platform);
            if(platform->props){
                platform->props->setPos(platform->pos().x() + 10 + rand() % 65, \
                                        platform->pos().y() - 15);
                scene->addItem(platform->props);
            }
        } else {
            delete platform;
            break;
        }
    }

    count = 0;
    if(score > 1000){
        int p = rand() % 100;
        Hazard * hazard = NULL;
        if(p > 30){
            hazard = new Monster(timer);
            do{
                hazard->setPos(rand() % 470, rand() % 920 + begin);
                if(count++ > 1000){
                    break;
                }
            } while (isOverlapping(hazard));
        } else {
            hazard = new Hole(timer);
            do{
                hazard->setPos(rand() % 470, rand() % 920 + begin);
                if(count++ > 1000){
                    break;
                }
            } while (isOverlapping(hazard));
        }
        if(hazard){
            if(count < 1000){
                scene->addItem(hazard);
                hazards.push_back(hazard);
            } else {
                delete hazard;
            }
        }
    }
}

bool DoodleJump::isOverlapping(QGraphicsPixmapItem * target)
{
    int target_width;
    if(typeid (target) != typeid (HorizontalMovePlatform*)){
        target_width = 640;
    } else {
        target_width = target->pixmap().width();
    }
    for(auto plat: platforms){
        int plat_width;
        if(typeid (plat) != typeid (HorizontalMovePlatform*)){
            plat_width = 640;
        } else {
            plat_width = plat->pixmap().width();
        }
        // Determine if two rectangles overlap each other
        if(!((target->pos().x() > plat->pos().x() + plat_width)|| \
           (target->pos().x() + target_width < plat->pos().x()) || \
           (target->pos().y() > plat->pos().y() + plat->pixmap().height()) || \
           (target->pos().y() + target->pixmap().height() < plat->pos().y())))
        {
            return true;
        }

    }
    for(auto hazard: hazards){
        if(!((target->pos().x() > hazard->pos().x() + hazard->pixmap().width() )|| \
           (target->pos().x() + target_width < hazard->pos().x()) || \
           (target->pos().y() > hazard->pos().y() + hazard->pixmap().height()) || \
           (target->pos().y() + target->pixmap().height() < hazard->pos().y())))
        {
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
            if(platform->scene()) {
                scene->removeItem(platform);
            }
            platforms.erase(iter);
            delete platform;
        } else {
            iter++;
        }
    }

    for(QVector<Hazard *>::iterator iter = hazards.begin(); iter != hazards.end();){
        // kind of weird
        auto hazard = *iter;
        if(hazard->pos().y() >  view->mapToScene(QPoint(0, view->height())).y()){
            scene->removeItem(hazard);
            hazards.erase(iter);
            delete hazard;
        } else {
            iter++;
        }
    }
}

void DoodleJump::end()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(setBackGround()));
    disconnect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    endBar->setY(view->mapToScene(QPoint(0, view->height())).y() - 33);
    gameOver->setY(view->mapToScene(QPoint(0, 0)).y() + 330);
    tapToChange->setY(view->mapToScene(QPoint(0, 0)).y() + 660);
    if(score > highestScore){
        highestScore = score;
    }
    QString text;
    text += "     your score: " + QString::number(score);
    text += "\nyour high score: " + QString::number(highestScore);
    text += "\n   your name: ";
    endText->setPlainText(text);
    endText->setY(view->mapToScene(QPoint(0, 0)).y() + 480);
    nameText->setPlainText(name);
    nameText->setY(view->mapToScene(QPoint(0, 0)).y() + 605);
    playAgainButton->setY(view->mapToScene(QPoint(0, 0)).y() + 730);
    menuButton->setY(view->mapToScene(QPoint(0, 0)).y() + 820);

    scene->addItem(endBar);
    scene->addItem(gameOver);
    scene->addItem(tapToChange);
    scene->addItem(endText);
    scene->addItem(nameText);
    scene->addItem(playAgainButton);
    scene->addItem(menuButton);

    scoreData->clean(scene);
    scoreData->add(name, score);
//    scoreData->save();

    for(auto platform: platforms){
        delete platform;
    }
    platforms.clear();
    for(auto hazard: hazards){
        delete hazard;
    }
    hazards.clear();
}
