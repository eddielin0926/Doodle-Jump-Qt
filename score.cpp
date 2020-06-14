#include "score.h"

#include <QFile>
#include <QPixmap>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>

NameScore::NameScore(QString s)
{
    QStringList list = s.split('#');
    name = list.at(0);
    score = list.at(1).toInt();
}

Score::Score()
{
}

void Score::read()
{
    QFile file("/score");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fail to read file: score";
        return;
    }
    QTextStream in(&file);
    QString input = in.readAll();
    if(input != NULL){
        QStringList list = input.split('\n');
        for(auto str: list){
            NameScore ns(str);
            data.push_back(ns);
        }
    }
    file.close();
}

void Score::save()
{
    QFile file("/score");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Fail to save file: score";
        return;
    }
    QTextStream out(&file);
    for(auto ns: data){
        out << ns.name << '#' << ns.score << '\n';
    }
    file.close();
}

void Score::add(QString name, int score)
{
    NameScore ns(name + '#' + QString::number(score));
    data.push_back(ns);
}

int Score::findHighScore(QString name)
{
    int score = 0;
    for(auto ns: data){
        if(ns.name == name){
            if(ns.score > score){
                score = ns.score;
            }
        }
    }
    return score;
}

void Score::addToScene(QGraphicsScene * scene)
{
    int id = QFontDatabase::addApplicationFont(":/font/resource/DoodleJump.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    font.setBold(true);
    font.setPixelSize(20);

    QGraphicsTextItem * text = NULL;
    QGraphicsPixmapItem * line = NULL;
    for(auto ns: data){
        text = new QGraphicsTextItem;
        line = new QGraphicsPixmapItem(QPixmap(":/background/images/line.png"));

        text->setFont(font);
        text->setPlainText(ns.name);
        text->setPos(640-text->boundingRect().width(), -ns.score + 490);
        text->setZValue(2);
        line->setPos(640 - line->pixmap().width(), -ns.score + 520);
        line->setZValue(2);

        graphics.push_back(text);
        graphics.push_back(line);

        scene->addItem(text);
        scene->addItem(line);
    }
}

void Score::clean(QGraphicsScene * scene)
{
    for(auto graphic: graphics){
        scene->removeItem(graphic);
        delete  graphic;
    }
    graphics.clear();
}
