#ifndef SCORE_H
#define SCORE_H

#include <QString>
#include <QVector>
#include <QGraphicsScene>

class NameScore{
public:
    NameScore(QString);

    QString name;
    int score;
};

class Score
{
public:
    Score();

    void read();
    void save();
    void add(QString, int);
    int findHighScore(QString);
    void addToScene(QGraphicsScene *);
    void clean(QGraphicsScene *);

private:
    QVector<NameScore> data;
    QVector<QGraphicsItem *> graphics;
};

#endif // SCORE_H
