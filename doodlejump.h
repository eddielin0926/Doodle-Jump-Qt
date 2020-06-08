#ifndef DOODLEJUMP_H
#define DOODLEJUMP_H

#include "mainwindow.h"
#include "player.h"
#include "platform.h"
#include "button.h"
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

public slots:
    void play();
    void pause();
    void resume();
    void setBackGround();

private:
    bool isOverlapping(Platform *);
    bool possiability(float);

    int score, highestScore, last;
    QString name;
    Player * player;
    QGraphicsScene * scene;
    QTimer * timer;
    QGraphicsView * view;
    QGraphicsPixmapItem background[2];
    QVector<Platform *> platforms;
    QGraphicsPixmapItem welcomeBackground, pauseBackground, toolBar, endBar, gameOver;
    QGraphicsTextItem * scoreText, * endText;
    Button * playButton, * pauseButton, * resumeButton, * playAgainButton, * menuButton;
};

#endif // DOODLEJUMP_H
