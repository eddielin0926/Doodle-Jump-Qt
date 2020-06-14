#ifndef DOODLEJUMP_H
#define DOODLEJUMP_H

#include "mainwindow.h"
#include "player.h"
#include "platform.h"
#include "button.h"
#include "hazard.h"
#include "score.h"
#include <QObject>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QVector>
#include <QString>

class Player;
class Platform;
class Hazard;
class Score;

class DoodleJump:public QObject
{
    Q_OBJECT;

    friend class MainWindow; // not sure it is necessary

public:
    DoodleJump(QGraphicsView * = NULL);

    void spawnPlaform(int);
    void cleanPlatform();
    void end();

    bool isEnd;
    QTimer * timer;

public slots:
    void play();
    void pause();
    void resume();
    void menu();
    void setBackGround();

private:
    bool isOverlapping(QGraphicsPixmapItem *);
    bool possiability(float);

    int score, highestScore, last;
    QString name;
    Player * player;
    Score * scoreData;
    QGraphicsScene * scene;
    QGraphicsView * view;
    QGraphicsPixmapItem background[2];
    QVector<Platform *> platforms;
    QVector<Hazard *> hazards;

    QGraphicsPixmapItem welcomeBackground, pauseBackground, toolBar, endBar, gameOver, tapToChange;
    QGraphicsTextItem * scoreText, * endText, * nameText;
    Button * playButton, * pauseButton, * resumeButton, * playAgainButton, * menuButton;
};

#endif // DOODLEJUMP_H
